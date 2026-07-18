// Boot + main loop: the browser twin of gbgame's wWinMain/WndProc. The wasm
// core owns all game state; this file owns the devices — canvas, WebAudio,
// clock, input — and the navigation pump that fetches pages on demand.

import createGbWeb from "../engine/gbweb.js?v=27";
import { DataLoader } from "./loader.js";
import { AudioEngine } from "./audio.js";
import { VideoEngine } from "./video.js";
import { RecorderEngine } from "./recorder.js";
import { drawTextItems } from "./text.js";
import { attachInput } from "./input.js";

const MAX_FOLLOWS_PER_PUMP = 8;   // gbgame's kMaxFollowsPerPump

const dataRoot = document.body.dataset.gameDataRoot;
if (!dataRoot) {
  throw new Error("Player page is missing data-game-data-root");
}

const canvas = document.getElementById("screen");
const ctx = canvas.getContext("2d");
const overlay = document.getElementById("overlay");
const progress = document.getElementById("progress");
const startBtn = document.getElementById("start");
const hud = document.getElementById("hud");

const audio = new AudioEngine();
let video = null;
let recorder = null;
let Module = null;
let api = null;
let loader = null;
let navigating = false;   // a page fetch is in flight; pause the clock
let lastTick = 0;
let animationFrameId = 0;
let baseFrame = null;
// PlaySynchroText is emitted by the recovered Sound_Holder.EndPlaySound
// callback, so it already begins after the title/intro clip. A second delay
// here would shift the karaoke display and invite duplicate-start races.
const TEXT_NARRATION_DELAY_MS = 0;
const delayedMediaStarts = new Map();

function textProgress(item, key) {
  if (!item.playing) return -1;
  const synchronizedProgress = audio.syncProgress(key);
  if (synchronizedProgress >= 0) return synchronizedProgress;
  const decodedProgress = audio.progress(key);
  // External .EXS narration is decoded asynchronously. Hold the first line
  // until WebAudio exposes the actual source start time; an optimistic timer
  // would advance, then visibly jump back to 0 when decoding completes.
  return decodedProgress >= 0 ? decodedProgress : 0;
}

function setProgress(msg) { progress.textContent = msg; }

function reportStatus() {
  const status = api.status();
  if (status) {
    hud.textContent = `${api.currentPage()} — [${status}]`;
    console.warn("gbweb:", status);
  } else {
    hud.textContent = api.currentPage();
  }
}

function drainAudio() {
  const events = JSON.parse(api.drainEvents());
  for (const ev of events) {
    const onEnded = ev.textEnd ? () => {
      if (!api || navigating) return;
      api.textAudioEnd(ev.id);
      drainAudio();
      pumpNavigation();
      blitIfDirty();
    } : null;
    if (ev.type === "play") {
      // Copy synchronously: the pointer is only valid until the next drain.
      const pcm = Module.HEAPU8.slice(ev.ptr, ev.ptr + ev.len);
      audio.play(ev.key, pcm, ev.rate, ev.bits, ev.channels, ev.loop, onEnded);
    } else if (ev.type === "playMedia") {
      const url = loader.assetUrl(ev.file);
      if (url) {
        if (ev.textEnd) {
          if (!audio.playing.has(ev.key)) {
            const previous = delayedMediaStarts.get(ev.key);
            if (previous) window.clearTimeout(previous);
            const timer = window.setTimeout(() => {
              delayedMediaStarts.delete(ev.key);
              if (!audio.playing.has(ev.key)) {
                audio.playMedia(ev.key, url, 0, null, onEnded,
                  { sanitizeSyncMarkers: Boolean(ev.textEnd) });
              }
            }, TEXT_NARRATION_DELAY_MS);
            delayedMediaStarts.set(ev.key, timer);
          }
        } else {
          audio.playMedia(ev.key, url, 0, null, onEnded,
            { sanitizeSyncMarkers: Boolean(ev.textEnd) });
        }
      }
      else console.error(`Text_Holder asset is not packaged: ${ev.file}`);
    } else if (ev.type === "stop") {
      const timer = delayedMediaStarts.get(ev.key);
      if (timer) { window.clearTimeout(timer); delayedMediaStarts.delete(ev.key); }
      audio.stop(ev.key);
    } else if (ev.type === "stopComponent") {
      for (const [key, timer] of delayedMediaStarts) {
        if (key.startsWith(ev.key)) { window.clearTimeout(timer); delayedMediaStarts.delete(key); }
      }
      audio.stopComponent(ev.key);
    } else if (ev.type === "stopAll") {
      for (const timer of delayedMediaStarts.values()) window.clearTimeout(timer);
      delayedMediaStarts.clear();
      audio.stopAll();
      video?.stopAll();
      recorder?.stop();
    } else if (ev.type === "volume") {
      audio.setVolume(ev.key, ev.value);
    } else if (ev.type === "recorder") {
      recorder?.dispatch(ev);
    } else if (ev.type === "playVideo") {
      video?.play(ev);
    } else if (ev.type === "stopVideoComponent") {
      video?.stopComponent(ev.key);
    }
  }
}

function fitCanvas() {
  const stage = document.getElementById("stage");
  const scale = Math.min(
    stage.clientWidth / canvas.width,
    stage.clientHeight / canvas.height,
  );
  canvas.style.width = `${Math.floor(canvas.width * scale)}px`;
  canvas.style.height = `${Math.floor(canvas.height * scale)}px`;
  video?.fit();
}

function blitIfDirty(forceText = false) {
  const rendered = api.render();
  const w = api.frameW();
  const h = api.frameH();
  if (w <= 0 || h <= 0) return;
  if (canvas.width !== w || canvas.height !== h) {
    canvas.width = w;
    canvas.height = h;
    fitCanvas();
  }
  if (rendered) {
    const ptr = api.framePtr();
    const pixels = new Uint8ClampedArray(Module.HEAPU8.buffer, ptr, w * h * 4);
    baseFrame = new ImageData(pixels.slice(), w, h);
  }
  if (!baseFrame || (!rendered && !forceText)) return;
  ctx.putImageData(baseFrame, 0, 0);
  const textItems = JSON.parse(api.textItems());
  drawTextItems(ctx, textItems, (key, item) => item ? textProgress(item, key) : -1);
  canvas.style.cursor = api.cursorHidden() ? "none" : "default";
}

function endGame(message) {
  cancelAnimationFrame(animationFrameId);
  audio.stopAll();
  overlay.classList.remove("hidden");
  overlay.querySelector("h1").textContent = message;
  startBtn.style.display = "none";
  setProgress("");
}

// Follow script LoadPage requests, fetching each target .BDF first. Bounded
// per pump so a page cycle cannot wedge the loop (mirrors pumpNavigation).
async function pumpNavigation() {
  if (navigating) return;
  for (let i = 0; i < MAX_FOLLOWS_PER_PUMP; i++) {
    if (api.exited()) {
      endGame("Do zobaczenia!");
      return;
    }
    const pending = api.pendingPage();
    if (!pending) return;
    const previous = api.currentPage();
    navigating = true;
    try {
      await loader.ensure(pending);
      if (!api.followPending()) {
        reportStatus();
        return;
      }
    } catch (err) {
      hud.textContent = String(err);
      console.error(err);
      return;
    } finally {
      navigating = false;
      lastTick = performance.now();   // don't bill fetch time to the game clock
    }
    if (previous && previous.toLowerCase() !== pending.toLowerCase()) {
      loader.release(previous);
    }
    drainAudio();   // the new page's OnOpenPage may have started sounds
    blitIfDirty();
    reportStatus();
  }
}

function onAnimationFrame(now) {
  animationFrameId = requestAnimationFrame(onAnimationFrame);
  if (navigating) {
    lastTick = now;
    return;
  }
  const elapsed = Math.round(now - lastTick);
  lastTick = now;
  api.tick(elapsed);
  drainAudio();
  pumpNavigation();
  blitIfDirty(true);
}

async function boot() {
  Module = await createGbWeb({
    locateFile: (file) => "engine/" + file + "?v=27",
  });
  const cw = (name, ret, args) => Module.cwrap(name, ret, args);
  api = {
    loadProject: cw("gb_load_project", "number", ["string"]),
    startupPage: cw("gb_startup_page", "string", []),
    openStartup: cw("gb_open_startup", "number", []),
    openPage: cw("gb_open_page", "number", ["string"]),
    exited: cw("gb_exited", "number", []),
    status: cw("gb_status", "string", []),
    currentPage: cw("gb_current_page", "string", []),
    pendingPage: cw("gb_pending_page", "string", []),
    followPending: cw("gb_follow_pending", "number", []),
    tick: cw("gb_tick", null, ["number"]),
    mouseMove: cw("gb_mouse_move", null, ["number", "number"]),
    mouseLeave: cw("gb_mouse_leave", null, []),
    mouseDown: cw("gb_mouse_down", null, ["number", "number"]),
    mouseUp: cw("gb_mouse_up", null, ["number", "number"]),
    mouseRDown: cw("gb_mouse_rdown", null, ["number", "number"]),
    keyDown: cw("gb_key_down", null, ["number"]),
    keyUp: cw("gb_key_up", null, ["number"]),
    externalVideoEnd: cw("gb_external_video_end", null, ["number"]),
    textAudioEnd: cw("gb_text_audio_end", null, ["number"]),
    recorderEndRecord: cw("gb_recorder_end_record", null, ["number"]),
    recorderEndPlay: cw("gb_recorder_end_play", null, []),
    recorderProgress: cw("gb_recorder_progress", null, ["number"]),
    render: cw("gb_render", "number", []),
    framePtr: cw("gb_frame_ptr", "number", []),
    frameW: cw("gb_frame_w", "number", []),
    frameH: cw("gb_frame_h", "number", []),
    cursorHidden: cw("gb_cursor_hidden", "number", []),
    drainEvents: cw("gb_drain_events", "string", []),
    textItems: cw("gb_text_items_json", "string", []),
  };

  recorder = new RecorderEngine({
    endRecord: (hasData) => { api.recorderEndRecord(hasData ? 1 : 0); drainAudio(); blitIfDirty(true); },
    endPlay: () => { api.recorderEndPlay(); drainAudio(); blitIfDirty(true); },
    progress: (percent) => { api.recorderProgress(percent); blitIfDirty(true); },
  });

  loader = new DataLoader(Module, dataRoot, setProgress);
  await loader.init();
  video = new VideoEngine(document.getElementById("videoLayer"), canvas, loader, audio, (id) => {
    if (!api || navigating) return;
    api.externalVideoEnd(id);
    drainAudio();
    pumpNavigation();
    blitIfDirty();
  });
  setProgress("Wczytywanie danych gry…");
  await loader.preloadBoot();

  if (!api.loadProject("/data/START.PRJ")) {
    throw new Error("START.PRJ: " + api.status());
  }
  const pageParam = new URL(window.location.href).searchParams.get("page");
  const requestedPage = pageParam
    ? (pageParam.toLowerCase().endsWith(".bdf") ? pageParam : `${pageParam}.bdf`)
    : "";
  await loader.ensure(requestedPage || api.startupPage());

  setProgress("");
  startBtn.disabled = false;
  startBtn.addEventListener("click", async () => {
    audio.unlock();
    overlay.classList.add("hidden");
    if (!(requestedPage ? api.openPage(requestedPage) : api.openStartup())) {
      endGame("Błąd: " + api.status());
      return;
    }
    drainAudio();
    blitIfDirty();
    reportStatus();
    await pumpNavigation();   // OnOpenPage may already have queued a LoadPage
    lastTick = performance.now();
    animationFrameId = requestAnimationFrame(onAnimationFrame);
  }, { once: true });
}

attachInput(canvas, {
  mouseMove: (x, y) => api && !navigating && api.mouseMove(x, y),
  mouseLeave: () => api && api.mouseLeave(),
  mouseDown: (x, y) => { if (api && !navigating) { audio.unlock(); api.mouseDown(x, y); drainAudio(); pumpNavigation(); blitIfDirty(); } },
  mouseUp: (x, y) => { if (api && !navigating) { api.mouseUp(x, y); drainAudio(); pumpNavigation(); blitIfDirty(); } },
  mouseRDown: (x, y) => { if (api && !navigating) { audio.unlock(); api.mouseRDown(x, y); drainAudio(); pumpNavigation(); blitIfDirty(); } },
  keyDown: (vk) => { if (api && !navigating) { audio.unlock(); api.keyDown(vk); drainAudio(); pumpNavigation(); blitIfDirty(); } },
  keyUp: (vk) => { if (api && !navigating) { api.keyUp(vk); drainAudio(); pumpNavigation(); blitIfDirty(); } },
});

window.addEventListener("resize", fitCanvas);
document.getElementById("fullscreen").addEventListener("click", () => {
  document.getElementById("stage").requestFullscreen?.();
});

boot().catch((err) => {
  setProgress(String(err));
  console.error(err);
});
