// Game-data provisioning: the wasm core reads original .PRJ/.BDF/.GRP files
// from Emscripten's MEMFS at /data. This module fetches them lazily from the
// static server and mirrors the shipped DATA folder's canonical filenames so
// the core's case-insensitive lookups behave exactly as they do on disk.

export class DataLoader {
  /**
   * @param {object} Module - the Emscripten module (for Module.FS)
   * @param {string} baseUrl - URL prefix of the packaged data dir, e.g. "data/tuwim/"
   * @param {(msg: string) => void} onProgress
   */
  constructor(Module, baseUrl, onProgress) {
    this.Module = Module;
    this.baseUrl = baseUrl;
    this.onProgress = onProgress || (() => {});
    this.manifest = new Map();   // lowercase name -> {file, size}
    this.resident = new Set();   // canonical filenames currently in MEMFS
  }

  async init() {
    const res = await fetch(this.baseUrl + "manifest.json");
    if (!res.ok) throw new Error(`manifest.json: HTTP ${res.status}`);
    const entries = await res.json();
    for (const entry of entries.files) {
      this.manifest.set(entry.file.toLowerCase(), entry);
    }
    this.Module.FS.mkdir("/data");
  }

  lookup(name) {
    return this.manifest.get(name.toLowerCase()) || null;
  }

  /** Browser media produced from a legacy external asset (currently AVI -> MP4). */
  mediaInfo(name) {
    const entry = this.lookup(name);
    if (!entry?.media) return null;
    const encoded = String(entry.media).split("/").map(encodeURIComponent).join("/");
    const audioEncoded = entry.audio
      ? String(entry.audio).split("/").map(encodeURIComponent).join("/")
      : "";
    return {
      ...entry,
      url: this.baseUrl + encoded,
      audioUrl: audioEncoded ? this.baseUrl + audioEncoded : "",
    };
  }

  /** URL for a manifest-backed asset that the browser can consume directly. */
  assetUrl(name) {
    const entry = this.lookup(name);
    if (!entry) return "";
    return this.baseUrl + String(entry.file).split("/").map(encodeURIComponent).join("/");
  }

  /** Fetch one file into MEMFS (no-op if already resident). Returns canonical path. */
  async ensure(name) {
    const entry = this.lookup(name);
    if (!entry) throw new Error(`plik nie istnieje w manifeście: ${name}`);
    const path = "/data/" + entry.file;
    if (this.resident.has(entry.file)) return path;
    const mb = entry.size >= 1 << 20 ? ` (${(entry.size / (1 << 20)).toFixed(1)} MB)` : "";
    this.onProgress(`Wczytywanie ${entry.file}${mb}…`);
    const res = await fetch(this.baseUrl + encodeURIComponent(entry.file));
    if (!res.ok) throw new Error(`${entry.file}: HTTP ${res.status}`);
    const bytes = new Uint8Array(await res.arrayBuffer());
    this.Module.FS.writeFile(path, bytes);
    this.resident.add(entry.file);
    this.onProgress("");
    return path;
  }

  /** Drop a previously fetched page file so the wasm heap stays bounded. */
  release(name) {
    const entry = this.lookup(name);
    if (!entry || !this.resident.has(entry.file)) return;
    // Groups stay resident for the whole session: LoadGroup runs mid-script
    // and cannot wait for a fetch.
    if (entry.file.toUpperCase().endsWith(".GRP")) return;
    if (entry.file.toUpperCase().endsWith(".PRJ")) return;
    try {
      this.Module.FS.unlink("/data/" + entry.file);
      this.resident.delete(entry.file);
    } catch {
      // best-effort
    }
  }

  /** Preload START.PRJ plus every .GRP (they are read synchronously mid-script). */
  async preloadBoot() {
    await this.ensure("START.PRJ");
    const groups = [...this.manifest.values()]
      .filter((e) => e.file.toUpperCase().endsWith(".GRP"));
    for (const entry of groups) {
      await this.ensure(entry.file);
    }
  }
}
