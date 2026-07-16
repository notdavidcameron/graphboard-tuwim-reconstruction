// HTMLVideoElement device layer for the legacy external Video_Holder AVIs.
// The packaging step transcodes Indeo 3.2 clips to browser-decodable MP4 while
// retaining the holder's script-visible filename, frame rate and stage origin.

export class VideoEngine {
  constructor(layer, canvas, loader, audio, onEnded) {
    this.layer = layer;
    this.canvas = canvas;
    this.loader = loader;
    this.audio = audio;
    this.onEnded = onEnded;
    this.playing = new Map();
  }

  fit() {
    const stageRect = this.layer.parentElement.getBoundingClientRect();
    const canvasRect = this.canvas.getBoundingClientRect();
    const scaleX = canvasRect.width / this.canvas.width;
    const scaleY = canvasRect.height / this.canvas.height;
    for (const item of this.playing.values()) {
      item.element.style.left = `${canvasRect.left - stageRect.left + item.x * scaleX}px`;
      item.element.style.top = `${canvasRect.top - stageRect.top + item.y * scaleY}px`;
      item.element.style.width = `${item.width * scaleX}px`;
      item.element.style.height = `${item.height * scaleY}px`;
    }
  }

  play(event) {
    const media = this.loader.mediaInfo(event.file);
    if (!media) {
      console.error(`Video_Holder asset is not packaged: ${event.file}`);
      queueMicrotask(() => this.onEnded(event.id));
      return;
    }

    this.stop(event.key);
    const element = document.createElement("video");
    element.className = "external-video";
    element.src = media.url;
    element.preload = "auto";
    element.playsInline = true;
    element.muted = true;
    element.disablePictureInPicture = true;
    element.controls = false;
    element.dataset.videoId = String(event.id);

    const item = {
      element,
      id: event.id,
      x: event.x,
      y: event.y,
      width: Number(media.width) || 1,
      height: Number(media.height) || 1,
      fps: Number(media.fps) || 15,
      endTime: null,
      completed: false,
    };
    this.playing.set(event.key, item);
    this.layer.appendChild(element);
    this.fit();

    const from = Number(event.from);
    const to = Number(event.to);
    const hasRange = from >= 0 && to >= 0;
    const shouldPlay = !(hasRange && to === from);

    const complete = () => {
      if (item.completed || this.playing.get(event.key) !== item) return;
      item.completed = true;
      element.pause();
      this.audio.stop(`${event.key}:audio`);
      this.onEnded(item.id);
    };

    element.addEventListener("ended", complete);
    element.addEventListener("error", () => {
      console.error(`Unable to decode packaged video: ${event.file}`);
      complete();
    });
    element.addEventListener("timeupdate", () => {
      if (item.endTime !== null && element.currentTime + 0.01 >= item.endTime) {
        element.currentTime = item.endTime;
        complete();
      }
    });

    // Muted visual playback is allowed even when Video_Holder dispatch happens
    // after asynchronous metadata work. Audio runs through the WebAudio context
    // unlocked by the title's initial Graj button.
    const playPromise = shouldPlay ? element.play() : null;
    playPromise?.catch((error) => {
      console.error(`Video playback blocked for ${event.file}:`, error);
      complete();
    });

    const begin = () => {
      item.width = element.videoWidth || item.width;
      item.height = element.videoHeight || item.height;
      const startTime = hasRange ? from / item.fps : 0;
      item.endTime = hasRange && to > from ? to / item.fps : null;
      element.currentTime = Math.min(startTime, Number.isFinite(element.duration) ? element.duration : startTime);
      if (shouldPlay && media.audioUrl) {
        const duration = item.endTime !== null ? Math.max(0, item.endTime - startTime) : null;
        this.audio.playMedia(`${event.key}:audio`, media.audioUrl, startTime, duration);
      }
      this.fit();
      if (hasRange && to === from) {
        element.addEventListener("seeked", () => element.pause(), { once: true });
      }
    };

    if (element.readyState >= HTMLMediaElement.HAVE_METADATA) begin();
    else element.addEventListener("loadedmetadata", begin, { once: true });
  }

  stop(key) {
    const item = this.playing.get(key);
    if (!item) return;
    this.playing.delete(key);
    item.completed = true;
    this.audio.stop(`${key}:audio`);
    item.element.pause();
    item.element.removeAttribute("src");
    item.element.load();
    item.element.remove();
  }

  stopComponent(prefix) {
    for (const key of [...this.playing.keys()]) {
      if (key.startsWith(prefix)) this.stop(key);
    }
  }

  stopAll() {
    for (const key of [...this.playing.keys()]) this.stop(key);
  }
}
