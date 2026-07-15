// Boot + main loop: the browser twin of gbgame's wWinMain/WndProc. The wasm
// core owns all game state; this file owns the devices — canvas, WebAudio,
// clock, input — and the navigation pump that fetches pages on demand.

import createGbWeb from "../engine/gbweb.js";
import { DataLoader } from "./loader.js";
import { AudioEngine } from "./audio.js";
import { drawTextItems } from "./text.js";
import { attachInput } from "./input.js";

const TICK_MS = 33;               // gbgame's kTickMs
const MAX_FOLLOWS_PER_PUMP = 8;   // gbgame's kMaxFollowsPerPump

const canvas = document.getElementById("screen");
const ctx = canvas.getContext("2d");
const overlay = document.getElementById("overlay");
const progress = document.getElementById("progress");
const startBtn = document.getElementById("start");
const hud = document.getElementById("hud");

const audio = new AudioEngine();
let Module = null;
let api = null;
let loader = null;
let navigating = false;   // a page fetch is in flight; pause the clock
let lastTick = 0;
let intervalId = 0;

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
    if (ev.type === "play") {
      // Copy synchronously: the pointer is only valid until the next drain.
      const pcm = Module.HEAPU8.slice(ev.ptr, ev.ptr + ev.len);
      audio.play(ev.key, pcm, ev.rate, ev.bits, ev.channels);
    } else if (ev.type === "stop") {
      audio.stop(ev.key);
    } else if (ev.type === "stopComponent") {
      audio.stopComponent(ev.key);
    } else if (ev.type === "stopAll") {
      audio.stopAll();
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
}

function blitIfDirty() {
  if (!api.render()) return;
  const w = api.frameW();
  const h = api.frameH();
  if (w <= 0 || h <= 0) return;
  if (canvas.width !== w || canvas.height !== h) {
    canvas.width = w;
    canvas.height = h;
    fitCanvas();
  }
  const ptr = api.framePtr();
  const pixels = new Uint8ClampedArray(Module.HEAPU8.buffer, ptr, w * h * 4);
  ctx.putImageData(new ImageData(pixels.slice(), w, h), 0, 0);
  drawTextItems(ctx, JSON.parse(api.textItems()));
  canvas.style.cursor = api.cursorHidden() ? "none" : "default";
}

function endGame(message) {
  clearInterval(intervalId);
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

function onTick() {
  if (navigating) return;
  const now = performance.now();
  const elapsed = Math.round(now - lastTick);
  lastTick = now;
  api.tick(elapsed);
  drainAudio();
  pumpNavigation();
  blitIfDirty();
}

async function boot() {
  Module = await createGbWeb({
    locateFile: (file) => "engine/" + file,
  });
  const cw = (name, ret, args) => Module.cwrap(name, ret, args);
  api = {
    loadProject: cw("gb_load_project", "number", ["string"]),
    startupPage: cw("gb_startup_page", "string", []),
    openStartup: cw("gb_open_startup", "number", []),
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
    render: cw("gb_render", "number", []),
    framePtr: cw("gb_frame_ptr", "number", []),
    frameW: cw("gb_frame_w", "number", []),
    frameH: cw("gb_frame_h", "number", []),
    cursorHidden: cw("gb_cursor_hidden", "number", []),
    drainEvents: cw("gb_drain_events", "string", []),
    textItems: cw("gb_text_items_json", "string", []),
  };

  loader = new DataLoader(Module, "data/tuwim/", setProgress);
  await loader.init();
  setProgress("Wczytywanie danych gry…");
  await loader.preloadBoot();

  if (!api.loadProject("/data/START.PRJ")) {
    throw new Error("START.PRJ: " + api.status());
  }
  await loader.ensure(api.startupPage());

  setProgress("");
  startBtn.disabled = false;
  startBtn.addEventListener("click", async () => {
    audio.unlock();
    overlay.classList.add("hidden");
    if (!api.openStartup()) {
      endGame("Błąd: " + api.status());
      return;
    }
    drainAudio();
    blitIfDirty();
    reportStatus();
    await pumpNavigation();   // OnOpenPage may already have queued a LoadPage
    lastTick = performance.now();
    intervalId = setInterval(onTick, TICK_MS);
  }, { once: true });
}

attachInput(canvas, {
  mouseMove: (x, y) => api && !navigating && api.mouseMove(x, y),
  mouseLeave: () => api && api.mouseLeave(),
  mouseDown: (x, y) => { if (api && !navigating) { api.mouseDown(x, y); drainAudio(); pumpNavigation(); blitIfDirty(); } },
  mouseUp: (x, y) => { if (api && !navigating) { api.mouseUp(x, y); drainAudio(); pumpNavigation(); blitIfDirty(); } },
  mouseRDown: (x, y) => { if (api && !navigating) { api.mouseRDown(x, y); drainAudio(); pumpNavigation(); blitIfDirty(); } },
  keyDown: (vk) => { if (api && !navigating) { api.keyDown(vk); drainAudio(); pumpNavigation(); blitIfDirty(); } },
});

window.addEventListener("resize", fitCanvas);
document.getElementById("fullscreen").addEventListener("click", () => {
  document.getElementById("stage").requestFullscreen?.();
});

boot().catch((err) => {
  setProgress(String(err));
  console.error(err);
});
