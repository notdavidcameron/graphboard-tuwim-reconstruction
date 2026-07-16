// WebAudio replay of the shim's drained audio events — the browser equivalent
// of gbgame's AudioEngine: overlapping clips keyed "component/id" so a script
// Stop silences exactly the clip it names.

/**
 * Hide SynchroText's one-sample line markers without changing the stream
 * length. The original files are RIFF/WAVE PCM with an .EXS extension; the
 * marker is -32768 for 16-bit audio (or 255 for 8-bit audio), and is consumed
 * by the legacy synchronizer rather than sent to the speaker.
 */
export function concealSyncMarkersInWav(arrayBuffer) {
  if (!(arrayBuffer instanceof ArrayBuffer) || arrayBuffer.byteLength < 12) return arrayBuffer;
  const source = new DataView(arrayBuffer);
  const ascii = (offset, value) => {
    if (offset + value.length > source.byteLength) return false;
    for (let i = 0; i < value.length; i++) {
      if (source.getUint8(offset + i) !== value.charCodeAt(i)) return false;
    }
    return true;
  };
  if (!ascii(0, "RIFF") || !ascii(8, "WAVE")) return arrayBuffer;
  const riffEnd = Math.min(source.byteLength, source.getUint32(4, true) + 8);
  let pos = 12;
  let channels = 0;
  let bits = 0;
  let dataStart = -1;
  let dataBytes = 0;
  while (pos + 8 <= riffEnd) {
    const size = source.getUint32(pos + 4, true);
    if (size > riffEnd - pos - 8) break;
    if (ascii(pos, "fmt ") && size >= 16) {
      const format = source.getUint16(pos + 8, true);
      channels = source.getUint16(pos + 10, true);
      bits = source.getUint16(pos + 22, true);
      if (format !== 1) return arrayBuffer;
    } else if (ascii(pos, "data")) {
      dataStart = pos + 8;
      dataBytes = size;
    }
    pos += 8 + size + (size & 1);
  }
  if (dataStart < 0 || !channels || (bits !== 8 && bits !== 16)) return arrayBuffer;

  const result = arrayBuffer.slice(0);
  const view = new DataView(result);
  const bytesPerSample = bits / 8;
  const frameBytes = channels * bytesPerSample;
  const dataEnd = Math.min(result.byteLength, dataStart + dataBytes);
  const frames = Math.floor((dataEnd - dataStart) / frameBytes);
  const sampleOffset = (frame, channel) => dataStart + frame * frameBytes + channel * bytesPerSample;
  for (let frame = 0; frame < frames; frame++) {
    for (let channel = 0; channel < channels; channel++) {
      const offset = sampleOffset(frame, channel);
      const marker = bits === 8
        ? view.getUint8(offset) === 0xff
        : view.getInt16(offset, true) === -32768;
      if (!marker) continue;
      const previousFrame = frame > 0 ? frame - 1 : frame;
      const nextFrame = frame + 1 < frames ? frame + 1 : frame;
      const previousOffset = sampleOffset(previousFrame, channel);
      const nextOffset = sampleOffset(nextFrame, channel);
      if (bits === 8) {
        view.setUint8(offset, Math.round((view.getUint8(previousOffset) + view.getUint8(nextOffset)) / 2));
      } else {
        const previous = view.getInt16(previousOffset, true);
        const next = view.getInt16(nextOffset, true);
        view.setInt16(offset, Math.trunc((previous + next) / 2), true);
      }
    }
  }
  return result;
}

/** Return the original SynchroText cue positions in seconds. */
export function syncMarkerTimesInWav(arrayBuffer) {
  if (!(arrayBuffer instanceof ArrayBuffer) || arrayBuffer.byteLength < 12) return [];
  const view = new DataView(arrayBuffer);
  const ascii = (offset, value) => {
    if (offset + value.length > view.byteLength) return false;
    for (let i = 0; i < value.length; i++) {
      if (view.getUint8(offset + i) !== value.charCodeAt(i)) return false;
    }
    return true;
  };
  if (!ascii(0, "RIFF") || !ascii(8, "WAVE")) return [];
  const riffEnd = Math.min(view.byteLength, view.getUint32(4, true) + 8);
  let pos = 12;
  let channels = 0;
  let bits = 0;
  let sampleRate = 0;
  let dataStart = -1;
  let dataBytes = 0;
  while (pos + 8 <= riffEnd) {
    const size = view.getUint32(pos + 4, true);
    if (size > riffEnd - pos - 8) break;
    if (ascii(pos, "fmt ") && size >= 16) {
      if (view.getUint16(pos + 8, true) !== 1) return [];
      channels = view.getUint16(pos + 10, true);
      sampleRate = view.getUint32(pos + 12, true);
      bits = view.getUint16(pos + 22, true);
    } else if (ascii(pos, "data")) {
      dataStart = pos + 8;
      dataBytes = size;
    }
    pos += 8 + size + (size & 1);
  }
  if (dataStart < 0 || !channels || !sampleRate || (bits !== 8 && bits !== 16)) return [];
  const bytesPerSample = bits / 8;
  const frameBytes = channels * bytesPerSample;
  const dataEnd = Math.min(view.byteLength, dataStart + dataBytes);
  const frames = Math.floor((dataEnd - dataStart) / frameBytes);
  const markers = [];
  for (let frame = 0; frame < frames; frame++) {
    let marker = false;
    for (let channel = 0; channel < channels; channel++) {
      const offset = dataStart + frame * frameBytes + channel * bytesPerSample;
      if ((bits === 8 && view.getUint8(offset) === 0xff) ||
          (bits === 16 && view.getInt16(offset, true) === -32768)) {
        marker = true;
        break;
      }
    }
    if (marker) markers.push(frame / sampleRate);
  }
  return markers;
}

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
    source.onended = () => {
      if (this.playing.get(key) === source) {
        this.playing.delete(key);
        onEnded?.();
      }
    };
    source.start();
    this.playing.set(key, source);
  }

  async playMedia(key, url, offsetSeconds = 0, durationSeconds = null, onEnded = null,
                  { sanitizeSyncMarkers = false } = {}) {
    if (!this.context || !url) return;
    this.stop(key);
    const pending = { cancelled: false, stop() { this.cancelled = true; } };
    this.playing.set(key, pending);
    try {
      const response = await fetch(url);
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      let encoded = await response.arrayBuffer();
      const syncMarkers = sanitizeSyncMarkers ? syncMarkerTimesInWav(encoded) : [];
      if (sanitizeSyncMarkers) encoded = concealSyncMarkersInWav(encoded);
      const buffer = await this.context.decodeAudioData(encoded);
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
      source._gbOffset = offset;
      source._gbSyncMarkers = syncMarkers;
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
      try { source.stop(); } catch { /* already ended */ }
    }
  }

  gainFor(key) {
    const value = Math.max(-10000, Math.min(0, Number(this.volumes.get(key)) || 0));
    return value <= -10000 ? 0 : Math.pow(10, value / 2000);
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

  syncProgress(key) {
    const source = this.playing.get(key);
    if (!this.context || !source?._gbSyncMarkers?.length) return -1;
    const elapsed = (this.context.currentTime - source._gbStart) + (source._gbOffset || 0);
    let marker = 0;
    while (marker + 1 < source._gbSyncMarkers.length &&
           source._gbSyncMarkers[marker + 1] <= elapsed) {
      marker++;
    }
    return marker / source._gbSyncMarkers.length;
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
