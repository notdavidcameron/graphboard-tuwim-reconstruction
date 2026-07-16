// Browser implementation of Recorder.dll's small file-oriented API. Recordings
// stay in memory for the current session, keyed by the WAV names used by the
// original scripts. Microphone permission is requested only when Record is
// clicked on a recital page.

export class RecorderEngine {
  constructor(callbacks) {
    this.callbacks = callbacks;
    this.files = new Map();
    this.currentFile = "recording.wav";
    this.stream = null;
    this.recorder = null;
    this.chunks = [];
    this.player = null;
    this.playerUrl = "";
    this.progressTimer = 0;
    this.recordStarted = 0;
  }

  key(name = this.currentFile) {
    return String(name || "recording.wav").toLowerCase();
  }

  async dispatch(event) {
    if (event.file) this.currentFile = event.file;
    switch (event.action) {
      case "OpenFile": break;
      case "CloseFile": this.stop(); break;
      case "EmptyFile":
        this.files.delete(this.key());
        break;
      case "Record": await this.record(); break;
      case "Play": this.play(); break;
      case "Stop": this.stop(); break;
    }
  }

  async record() {
    this.stop();
    try {
      this.stream ||= await navigator.mediaDevices.getUserMedia({ audio: true });
      this.chunks = [];
      this.recorder = new MediaRecorder(this.stream);
      this.recorder.addEventListener("dataavailable", (event) => {
        if (event.data.size) this.chunks.push(event.data);
      });
      this.recorder.addEventListener("stop", () => {
        const blob = new Blob(this.chunks, { type: this.recorder?.mimeType || "audio/webm" });
        if (blob.size) this.files.set(this.key(), blob);
        this.clearProgress();
        this.callbacks.endRecord(blob.size > 0);
      }, { once: true });
      this.recorder.start(200);
      this.recordStarted = performance.now();
      this.progressTimer = window.setInterval(() => {
        const percent = Math.min(99, Math.floor((performance.now() - this.recordStarted) / 600));
        this.callbacks.progress(percent);
      }, 250);
    } catch (error) {
      console.error("Microphone recording is unavailable:", error);
      this.callbacks.endRecord(false);
    }
  }

  play() {
    this.stop();
    const blob = this.files.get(this.key());
    if (!blob) {
      this.callbacks.endPlay();
      return;
    }
    this.playerUrl = URL.createObjectURL(blob);
    this.player = new Audio(this.playerUrl);
    this.player.addEventListener("ended", () => {
      this.cleanupPlayer();
      this.callbacks.endPlay();
    }, { once: true });
    this.player.play().catch((error) => {
      console.error("Recorded audio playback failed:", error);
      this.cleanupPlayer();
      this.callbacks.endPlay();
    });
  }

  stop() {
    if (this.recorder?.state === "recording") {
      this.recorder.stop();
    }
    this.recorder = null;
    this.clearProgress();
    if (this.player) {
      this.player.pause();
      this.cleanupPlayer();
    }
  }

  clearProgress() {
    if (this.progressTimer) window.clearInterval(this.progressTimer);
    this.progressTimer = 0;
  }

  cleanupPlayer() {
    this.player = null;
    if (this.playerUrl) URL.revokeObjectURL(this.playerUrl);
    this.playerUrl = "";
  }
}
