// WebAudio replay of the shim's drained audio events — the browser equivalent
// of gbgame's AudioEngine: overlapping clips keyed "component/id" so a script
// Stop silences exactly the clip it names.

export class AudioEngine {
  constructor() {
    this.context = null;           // created on first user gesture
    this.playing = new Map();      // key -> AudioBufferSourceNode
  }

  /** Must be called from a user-gesture handler (autoplay policy). */
  unlock() {
    if (!this.context) {
      this.context = new (window.AudioContext || window.webkitAudioContext)();
    }
    if (this.context.state === "suspended") {
      this.context.resume();
    }
  }

  /**
   * @param {string} key
   * @param {Uint8Array} pcm - raw samples (WAV framing already stripped)
   */
  play(key, pcm, rate, bits, channels) {
    if (!this.context || !pcm.length || !rate || !channels) return;
    this.stop(key);
    const bytesPerSample = bits / 8;
    const frames = Math.floor(pcm.length / (bytesPerSample * channels));
    if (frames <= 0) return;
    const buffer = this.context.createBuffer(channels, frames, rate);
    for (let ch = 0; ch < channels; ch++) {
      const dest = buffer.getChannelData(ch);
      if (bits === 8) {
        for (let i = 0; i < frames; i++) {
          dest[i] = (pcm[i * channels + ch] - 128) / 128;
        }
      } else if (bits === 16) {
        const view = new DataView(pcm.buffer, pcm.byteOffset, pcm.byteLength);
        for (let i = 0; i < frames; i++) {
          dest[i] = view.getInt16((i * channels + ch) * 2, true) / 32768;
        }
      } else {
        return; // no other PCM widths ship in these titles
      }
    }
    const source = this.context.createBufferSource();
    source.buffer = buffer;
    source.connect(this.context.destination);
    source.onended = () => {
      if (this.playing.get(key) === source) this.playing.delete(key);
    };
    source.start();
    this.playing.set(key, source);
  }

  stop(key) {
    const source = this.playing.get(key);
    if (source) {
      this.playing.delete(key);
      try { source.stop(); } catch { /* already ended */ }
    }
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
