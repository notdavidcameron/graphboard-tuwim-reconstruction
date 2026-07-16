// WebAudio replay of the shim's drained audio events — the browser equivalent
// of gbgame's AudioEngine: overlapping clips keyed "component/id" so a script
// Stop silences exactly the clip it names.

export class AudioEngine {
  constructor() {
    this.context = null;           // created on first user gesture
    this.playing = new Map();      // key -> AudioBufferSourceNode
    this.volumes = new Map();      // DirectSound hundredths-of-a-decibel values
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
  play(key, pcm, rate, bits, channels, loop = false, onEnded = null) {
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
    source.loop = Boolean(loop);
    const gain = this.context.createGain();
    source.connect(gain).connect(this.context.destination);
    source._gbGain = gain;
    source._gbStart = this.context.currentTime;
    source._gbDuration = buffer.duration;
    this.configureEnvelope(source, gain, key, buffer.duration, loop, this.context.currentTime);
    source.onended = () => {
      if (this.playing.get(key) === source) {
        this.playing.delete(key);
        onEnded?.();
      }
    };
    source.start();
    this.playing.set(key, source);
  }

  async playMedia(key, url, offsetSeconds = 0, durationSeconds = null, onEnded = null) {
    if (!this.context || !url) return;
    this.stop(key);
    const pending = { cancelled: false, stop() { this.cancelled = true; } };
    this.playing.set(key, pending);
    try {
      const response = await fetch(url);
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      const buffer = await this.context.decodeAudioData(await response.arrayBuffer());
      if (pending.cancelled || this.playing.get(key) !== pending) return;
      const offset = Math.max(0, Math.min(Number(offsetSeconds) || 0, buffer.duration));
      const source = this.context.createBufferSource();
      source.buffer = buffer;
      const gain = this.context.createGain();
      source.connect(gain).connect(this.context.destination);
      source._gbGain = gain;
      source._gbStart = this.context.currentTime;
      source._gbDuration = Number(durationSeconds) > 0
        ? Math.min(Number(durationSeconds), buffer.duration - offset)
        : buffer.duration - offset;
      this.configureEnvelope(source, gain, key, source._gbDuration, false, this.context.currentTime);
      source.onended = () => {
        if (this.playing.get(key) === source) {
          this.playing.delete(key);
          onEnded?.();
        }
      };
      this.playing.set(key, source);
      if (Number(durationSeconds) > 0) source.start(0, offset, Number(durationSeconds));
      else source.start(0, offset);
    } catch (error) {
      if (this.playing.get(key) === pending) this.playing.delete(key);
      console.error(`Unable to decode external audio ${url}:`, error);
    }
  }

  stop(key) {
    const source = this.playing.get(key);
    if (source) {
      this.playing.delete(key);
      try {
        if (source._gbGain && this.context) {
          const now = this.context.currentTime;
          source._gbGain.gain.cancelScheduledValues(now);
          source._gbGain.gain.setValueAtTime(Math.max(0.0001, source._gbGain.gain.value), now);
          source._gbGain.gain.linearRampToValueAtTime(0.0001, now + 0.01);
          source.stop(now + 0.012);
        } else {
          source.stop();
        }
      } catch { /* already ended */ }
    }
  }

  gainFor(key) {
    const value = Math.max(-10000, Math.min(0, Number(this.volumes.get(key)) || 0));
    return value <= -10000 ? 0 : Math.pow(10, value / 2000);
  }

  configureEnvelope(source, gain, key, duration, loop, startAt) {
    const target = this.gainFor(key);
    const attack = 0.006;
    const release = 0.012;
    gain.gain.setValueAtTime(0.0001, startAt);
    gain.gain.linearRampToValueAtTime(target, startAt + attack);
    if (!loop && duration > attack + release) {
      gain.gain.setValueAtTime(target, startAt + duration - release);
      gain.gain.linearRampToValueAtTime(0.0001, startAt + duration);
    }
  }

  setVolume(key, value) {
    this.volumes.set(key, Number(value) || 0);
    const source = this.playing.get(key);
    if (source?._gbGain && this.context) {
      source._gbGain.gain.setTargetAtTime(this.gainFor(key), this.context.currentTime, 0.015);
    }
  }

  progress(key) {
    const source = this.playing.get(key);
    if (!this.context || !source?._gbDuration) return -1;
    return Math.max(0, Math.min(1, (this.context.currentTime - source._gbStart) / source._gbDuration));
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
