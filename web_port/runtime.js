class GraphBoardRuntime {
  constructor(stage, logTarget) {
    this.stage = stage;
    this.logTarget = logTarget;
    this.scene = null;
    this.groups = new Map();
    this.group = null;
    this.layers = new Map();
    this.audio = new Map();
    this.assets = new Map();
    this.hotspots = new Map();
    this.variables = new Map();
    this.loadedGroup = null;
    this.cursorId = 0;
    this.screenEnabled = true;
    this.callDepth = 0;
    this.onLoadPage = null;
    this.timers = [];
    this.videoTimers = new Map();
    this.returning = false;
    this.returnValue = undefined;
    this.recorder = { file: "", hasContent: false, recording: false, playing: false };
    this.puzzle = { active: false, drops: 0 };
    this.userInputDepth = 0;
    this.trustedCallbackDepth = 0;
    this.handlerStack = [];
  }

  reset(scene) {
    this.clearTimers();
    this.clearVideoTimers();
    this.scene = scene;
    this.group = null;
    this.layers.clear();
    this.audio.clear();
    this.assets.clear();
    this.hotspots.clear();
    this.variables.clear();
    this.loadedGroup = null;
    this.cursorId = 0;
    this.screenEnabled = true;
    this.recorder = { file: "", hasContent: false, recording: false, playing: false };
    this.puzzle = { active: false, drops: 0 };
    this.userInputDepth = 0;
    this.trustedCallbackDepth = 0;
    this.handlerStack = [];
    if (this.logTarget) this.logTarget.textContent = "";
    this.indexSceneAssets();
    this.indexExistingDom();
    this.log(`runtime ready: ${scene?.id || "no scene"}`);
  }

  setGroups(groups) {
    this.groups = new Map();
    for (const group of groups || []) {
      this.groups.set(this.groupKey(group.id), group);
      this.groups.set(this.groupKey(group.sourceGrp), group);
    }
    this.log(`groups ready: ${this.groups.size}`);
  }

  groupKey(name) {
    return String(name || "")
      .replace(/^.*[\\/]/, "")
      .replace(/\.grp$/i, "")
      .toUpperCase();
  }

  key(namespace, type, id) {
    return `${namespace}:${type}:${Number(id) || 0}`;
  }

  legacyKey(type, id) {
    return `${type}:${Number(id) || 0}`;
  }

  normalizeType(type) {
    return String(type || "").replace(/^Group\./, "");
  }

  indexAsset(namespace, component, asset) {
    const type = this.normalizeType(component.type);
    const id = Number(asset.id ?? 0);
    const ids = new Set([id]);
    if ((type === "Transparent_Video_Holder" || type === "Sprite_Holder") && id > 0 && id % 2 === 0) {
      ids.add(id / 2);
    }
    for (const itemId of ids) {
      this.assets.set(this.key(namespace, type, itemId), asset);
      if (namespace === "Page") this.assets.set(this.legacyKey(type, itemId), asset);
    }
  }

  indexSceneAssets() {
    for (const component of this.scene?.components || []) {
      for (const asset of component.assets || []) this.indexAsset("Page", component, asset);
    }
  }

  indexExistingDom() {
    for (const layer of this.stage.querySelectorAll("[data-runtime-layer='1']")) {
      this.indexLayer(layer);
    }
    if (typeof document === "undefined") return;
    for (const audio of document.querySelectorAll("#audioList audio")) {
      this.indexAudio(audio);
    }
  }

  indexLayer(layer) {
    const namespace = layer.dataset.runtimeNamespace || "Page";
    const type = this.normalizeType(layer.dataset.componentType);
    const runtimeId = Number(layer.dataset.runtimeId ?? layer.dataset.assetId ?? 0);
    const assetId = Number(layer.dataset.assetId ?? runtimeId);
    const ids = new Set([runtimeId, assetId]);
    for (const id of ids) {
      this.layers.set(this.key(namespace, type, id), layer);
      if (namespace === "Page") this.layers.set(this.legacyKey(type, id), layer);
    }
  }

  indexAudio(audio) {
    const namespace = audio.dataset.runtimeNamespace || "Page";
    const type = this.normalizeType(audio.dataset.componentType);
    const runtimeId = Number(audio.dataset.runtimeId ?? audio.dataset.assetId ?? 0);
    const assetId = Number(audio.dataset.assetId ?? runtimeId);
    for (const id of new Set([runtimeId, assetId])) {
      this.audio.set(this.key(namespace, type, id), audio);
      if (namespace === "Page") this.audio.set(this.legacyKey(type, id), audio);
    }
    if (!audio.dataset.runtimeIndexed) {
      audio.dataset.runtimeIndexed = "1";
      audio.addEventListener("ended", () => {
        const endedType = this.normalizeType(audio.dataset.componentType);
        const id = Number(audio.dataset.runtimeId ?? audio.dataset.assetId ?? 0);
        const ns = audio.dataset.runtimeNamespace || "Page";
        this.executeHandler(`${ns === "Group" ? "Group." : ""}${endedType}.EndPlaySound`, [id]);
        if (endedType === "Sound_Holder" && ns === "Page") this.executeHandler("Sound_Holder.EndPlaySound", [id]);
      });
    }
  }

  clearTimers() {
    for (const timer of this.timers) clearTimeout(timer);
    this.timers = [];
  }

  clearVideoTimers() {
    for (const timer of this.videoTimers.values()) clearTimeout(timer);
    this.videoTimers.clear();
  }

  log(message) {
    if (!this.logTarget) return;
    const line = `[${new Date().toLocaleTimeString()}] ${message}`;
    this.logTarget.textContent = `${line}\n${this.logTarget.textContent}`.slice(0, 9000);
  }

  idCandidates(type, id) {
    const value = Number(id) || 0;
    const candidates = [value, value * 2, Math.max(0, value - 1) * 2];
    if (type === "Sprite_Holder" && value > 0) candidates.push(value - 1);
    return [...new Set(candidates)];
  }

  findLayer(type, id, namespace = "Page", create = false) {
    const cleanType = this.normalizeType(type);
    for (const candidate of this.idCandidates(cleanType, id)) {
      const layer = this.layers.get(this.key(namespace, cleanType, candidate)) || (namespace === "Page" ? this.layers.get(this.legacyKey(cleanType, candidate)) : null);
      if (layer) return layer;
    }
    if (!create) return null;
    if (["Sprite_Holder", "Bitmap_Holder", "Text_Holder", "Puzzle", "Video_Holder"].includes(cleanType)) {
      return this.createSyntheticLayer(namespace, cleanType, Number(id) || 0, this.componentRect(cleanType, id, namespace));
    }
    return null;
  }

  componentRect(type, id, namespace = "Page") {
    const cleanType = this.normalizeType(type);
    const collection = namespace === "Group" ? this.group?.components : this.scene?.components;
    const components = (collection || []).filter((component) => this.normalizeType(component.type) === cleanType);
    if (components.length === 0) return null;
    const wanted = Number(id) || 0;
    const exact = components.find((component) => Number(component.index) === wanted);
    const component = exact || (components.length === 1 ? components[0] : null);
    const rect = component?.rect;
    if (!rect) return null;
    return {
      x: Number(rect.x) || 0,
      y: Number(rect.y) || 0,
      width: Number(rect.width) || 1,
      height: Number(rect.height) || 1,
    };
  }

  findAsset(type, id, namespace = "Page") {
    const cleanType = this.normalizeType(type);
    for (const candidate of this.idCandidates(cleanType, id)) {
      const asset = this.assets.get(this.key(namespace, cleanType, candidate)) || (namespace === "Page" ? this.assets.get(this.legacyKey(cleanType, candidate)) : null);
      if (asset) return asset;
    }
    return null;
  }

  findAudio(type, id, namespace = "Page") {
    const cleanType = this.normalizeType(type);
    for (const candidate of this.idCandidates(cleanType, id)) {
      const audio = this.audio.get(this.key(namespace, cleanType, candidate)) || (namespace === "Page" ? this.audio.get(this.legacyKey(cleanType, candidate)) : null);
      if (audio) return audio;
    }
    return null;
  }

  bindRuntimeEvents(element, type, id, namespace = "Page") {
    element.dataset.componentType = this.normalizeType(type);
    element.dataset.runtimeId = String(id);
    element.dataset.runtimeNamespace = namespace;
    element.addEventListener("mouseenter", () => this.dispatchComponentEvent(type, "MouseMoveIn", id, [], namespace));
    element.addEventListener("mouseleave", () => this.dispatchComponentEvent(type, "MouseMoveOut", id, [], namespace));
    element.addEventListener("mousedown", (event) => {
      if (event.button === 0) {
        this.runUserInput(() => {
          if (type === "Puzzle") this.executeHandler("Puzzle.MouseStartDrag");
          this.dispatchComponentEvent(type, "MouseClickOnDown", id, [], namespace);
        });
      }
    });
    element.addEventListener("mouseup", (event) => {
      if (event.button === 0) {
        this.runUserInput(() => {
          if (type === "Puzzle") {
            this.puzzle.drops += 1;
            this.executeHandler("Puzzle.MouseDrop");
            if (this.puzzle.drops >= 3) this.executeHandler("Puzzle.GameOver", [id]);
          }
          this.dispatchComponentEvent(type, "MouseClickOnUp", id, [], namespace);
        });
      }
    });
    element.addEventListener("click", (event) => {
      event.stopPropagation();
      this.runUserInput(() => {
        if (type === "Text_Holder") this.dispatchComponentEvent(type, "ClickOnText", id, [], namespace);
        if (type === "HotSpot_Holder") this.dispatchComponentEvent(type, "LeftButtonClickOn", id, [], namespace);
      });
    });
  }

  runUserInput(callback) {
    this.userInputDepth += 1;
    try {
      return callback();
    } finally {
      this.userInputDepth -= 1;
    }
  }

  createSyntheticLayer(namespace, type, id, rect = null) {
    const layer = document.createElement(type === "Text_Holder" ? "div" : "button");
    layer.type = "button";
    layer.className = type === "Text_Holder" ? "text-layer runtime-hidden" : "synthetic-layer layer runtime-hidden";
    layer.dataset.runtimeLayer = "1";
    layer.dataset.synthetic = "1";
    layer.dataset.componentType = type;
    layer.dataset.runtimeNamespace = namespace;
    layer.dataset.runtimeId = String(id);
    layer.dataset.assetId = String(id);
    layer.dataset.runtimeKey = this.key(namespace, type, id);
    layer.title = `${namespace} ${type} ${id}`;
    if (namespace === "Group" && type === "Sprite_Holder") {
      const labels = {
        0: "",
        1: "Home",
        2: "<",
        3: ">",
        4: "R",
        5: "C",
        6: "P",
        7: "Home",
        8: "Easy",
        9: "Hard",
        10: "Back",
        11: "Text",
      };
      layer.dataset.toolbar = "1";
      layer.dataset.toolbarLabel = labels[id] ?? String(id);
      layer.title = layer.dataset.toolbarLabel ? `${layer.dataset.toolbarLabel} toolbar button` : "Toolbar";
      if (layer.dataset.toolbarLabel) layer.textContent = layer.dataset.toolbarLabel;
    }
    const defaultRect = rect || { x: 0, y: 0, width: type === "Puzzle" ? 580 : 48, height: type === "Puzzle" ? 480 : 48 };
    layer.style.left = `${defaultRect.x}px`;
    layer.style.top = `${defaultRect.y}px`;
    layer.style.width = `${Math.max(8, defaultRect.width)}px`;
    layer.style.height = `${Math.max(8, defaultRect.height)}px`;
    layer.style.zIndex = String(namespace === "Group" && type === "Sprite_Holder" ? 820000 + id : namespace === "Group" ? 600000 + id : 300000 + id);
    if (type === "Text_Holder") layer.textContent = "";
    this.bindRuntimeEvents(layer, type, id, namespace);
    this.stage.appendChild(layer);
    this.indexLayer(layer);
    return layer;
  }

  setLayerVisible(type, id, visible, namespace = "Page") {
    const layer = this.findLayer(type, id, namespace, visible);
    if (!layer) {
      this.log(`${namespace}.${type}.${visible ? "show" : "hide"}(${id}) missing layer`);
      return;
    }
    layer.classList.toggle("runtime-hidden", !visible);
    this.log(`${namespace}.${type}.${visible ? "show" : "hide"}(${id})`);
  }

  setLayerStill(type, id, first = true, namespace = "Page") {
    const layer = this.findLayer(type, id, namespace);
    if (!layer) return;
    layer.dataset.videoState = first ? "first" : "last";
    layer.classList.remove("runtime-playing");
    layer.classList.toggle("runtime-hidden", false);
    if (first) {
      if (layer.dataset.stillSrc) {
        layer.src = layer.dataset.stillSrc;
      } else if (layer.dataset.originalSrc) {
        layer.src = layer.dataset.originalSrc;
        setTimeout(() => {
          if (layer.dataset.videoState === "first") this.freezeLayer(layer, "first");
        }, 80);
      }
    }
  }

  restartLayerAnimation(layer) {
    if (!layer || !layer.getAttribute) return;
    const src = layer.dataset.originalSrc || layer.getAttribute("src");
    layer.dataset.originalSrc = src || "";
    layer.dataset.videoState = "playing";
    layer.classList.add("runtime-playing");
    layer.classList.remove("runtime-hidden");
    if (src) {
      layer.setAttribute("src", "");
      layer.offsetHeight;
      layer.setAttribute("src", src);
    }
  }

  freezeLayer(layer, state = "last") {
    if (!layer || layer.tagName !== "IMG" || !layer.complete || layer.naturalWidth <= 0 || layer.naturalHeight <= 0) return;
    try {
      const canvas = document.createElement("canvas");
      canvas.width = layer.naturalWidth;
      canvas.height = layer.naturalHeight;
      const context = canvas.getContext("2d");
      context.drawImage(layer, 0, 0);
      layer.src = canvas.toDataURL("image/png");
      layer.dataset.videoState = state;
    } catch (error) {
      this.log(`freeze skipped: ${error.message}`);
    }
  }

  moveLayer(type, id, x, y, z = undefined, namespace = "Page") {
    const layer = this.findLayer(type, id, namespace, true);
    if (!layer) {
      this.log(`${namespace}.${type}.move(${id},${x},${y}) missing layer`);
      return;
    }
    layer.style.left = `${Number(x) || 0}px`;
    layer.style.top = `${Number(y) || 0}px`;
    layer.style.transition = "left 180ms linear, top 180ms linear";
    if (Number.isFinite(Number(z))) layer.style.zIndex = String((namespace === "Group" ? 600000 : 100000) + Number(z));
    layer.classList.remove("runtime-hidden");
    this.log(`${namespace}.${type}.move(${id},${x},${y}${Number.isFinite(Number(z)) ? `,${z}` : ""})`);

    const prefix = namespace === "Group" ? "Group." : "";
    const cleanType = this.normalizeType(type);
    clearTimeout(layer._inPlaceTimer);
    const inPlaceHandler = `${prefix}${cleanType}.InPlace`;
    if (namespace === "Group" && this.extractFunctionBody(this.scene?.script?.rawText || "", inPlaceHandler)) {
      const trusted = this.userInputDepth > 0;
      layer._inPlaceTimer = setTimeout(() => {
        if (trusted) this.trustedCallbackDepth += 1;
        try {
          this.executeHandler(inPlaceHandler, [Number(id) || 0, Number(x) || 0, Number(y) || 0]);
        } finally {
          if (trusted) this.trustedCallbackDepth -= 1;
        }
      }, 210);
    }
  }

  changePhase(type, id, phase, namespace = "Page") {
    const layer = this.findLayer(type, id, namespace, true);
    if (layer) layer.dataset.phase = String(phase);
    this.log(`${namespace}.${type}.ChangePhase(${id},${phase})`);
  }

  setDeep(type, id, z, namespace = "Page") {
    const layer = this.findLayer(type, id, namespace, true);
    if (layer && Number.isFinite(Number(z))) layer.style.zIndex = String((namespace === "Group" ? 600000 : 100000) + Number(z));
  }

  playAudio(type, id, namespace = "Page") {
    const audio = this.findAudio(type, id, namespace);
    if (!audio) {
      this.log(`${namespace}.${type}.PlayDSound(${id}) missing audio`);
      return;
    }
    for (const item of this.audio.values()) {
      if (item !== audio && item.dataset.componentType === this.normalizeType(type) && item.dataset.runtimeNamespace === namespace) item.pause();
    }
    audio.currentTime = 0;
    audio.play().catch((error) => this.log(`audio blocked: ${error.message}`));
    this.log(`${namespace}.${type}.PlayDSound(${id})`);
  }

  stopAudio(type, id = undefined, namespace = "Page") {
    const audio = Number.isFinite(Number(id)) ? this.findAudio(type, Number(id), namespace) : null;
    const targets = audio
      ? [audio]
      : [...this.audio.values()].filter((item) => item.dataset.componentType === this.normalizeType(type) && item.dataset.runtimeNamespace === namespace);
    for (const item of targets) {
      item.pause();
      item.currentTime = 0;
    }
    this.log(`${namespace}.${type}.Stop(${Number.isFinite(Number(id)) ? id : "all"})`);
  }

  videoDurationMs(type, id, namespace = "Page") {
    const asset = this.findAsset(type, id, namespace);
    const frameDuration = Number(asset?.frameCount || 1) * 100;
    const matchingAudio = this.findAudio(type, id, namespace);
    const audioDuration = Number(matchingAudio?.dataset.durationSeconds || 0) * 1000;
    return Math.max(350, audioDuration || frameDuration || 3500);
  }

  playVideo(type, id, namespace = "Page") {
    const cleanType = this.normalizeType(type);
    const layer = this.findLayer(cleanType, id, namespace, cleanType === "Video_Holder");
    const key = this.key(namespace, cleanType, id);
    if (this.videoTimers.has(key)) clearTimeout(this.videoTimers.get(key));
    if (layer) this.restartLayerAnimation(layer);
    this.playAudio(cleanType, id, namespace);
    const delay = cleanType === "Video_Holder" ? 4000 : this.videoDurationMs(cleanType, id, namespace);
    this.videoTimers.set(key, setTimeout(() => {
      this.videoTimers.delete(key);
      if (layer) {
        layer.classList.remove("runtime-playing");
        this.freezeLayer(layer, "last");
      }
      this.executeHandler(`${namespace === "Group" ? "Group." : ""}${cleanType}.TheEnd`, [id]);
    }, delay));
    this.log(`${namespace}.${cleanType}.Play(${id}) for ${Math.round(delay)}ms`);
  }

  enableHotspot(id, enabled, namespace = "Page") {
    this.hotspots.set(this.key(namespace, "HotSpot_Holder", id), enabled);
    for (const target of this.stage.querySelectorAll(`[data-component-type="HotSpot_Holder"][data-runtime-namespace="${namespace}"][data-runtime-id="${id}"]`)) {
      target.classList.toggle("runtime-disabled", !enabled);
    }
    this.log(`${namespace}.HotSpot ${id} ${enabled ? "enabled" : "disabled"}`);
  }

  dispatchComponentEvent(type, eventName, id, extra = [], namespace = "Page") {
    const cleanType = this.normalizeType(type);
    const hotKey = this.key(namespace, "HotSpot_Holder", id);
    if (cleanType === "HotSpot_Holder" && this.hotspots.has(hotKey) && !this.hotspots.get(hotKey)) {
      this.log(`${namespace}.HotSpot ${id} ignored (${eventName})`);
      return;
    }
    const names = namespace === "Group"
      ? [`Group.${cleanType}.${eventName}`, `${cleanType}.${eventName}`]
      : [`${cleanType}.${eventName}`, `Group.${cleanType}.${eventName}`];
    for (const name of names) {
      if (this.extractFunctionBody(this.scene?.script?.rawText || "", name)) {
        this.executeHandler(name, [id, ...extra]);
        return;
      }
    }
    this.log(`event ${namespace}.${cleanType}.${eventName}(${[id, ...extra].join(",")})`);
  }

  executeHandler(name, args = []) {
    const script = this.scene?.script?.rawText || "";
    const body = this.extractFunctionBody(script, name);
    if (!body) {
      this.log(`handler not found: ${name}`);
      return undefined;
    }
    if (this.callDepth > 20) {
      this.log(`handler recursion limit: ${name}`);
      return undefined;
    }
    this.log(`run ${name}(${args.join(",")})`);
    for (const [key, index] of Object.entries({ rectID: 0, videoID: 0, spriteID: 0, soundID: 0, textID: 0, puzzleID: 0, bitmapID: 0, x: 1, y: 2 })) {
      if (args[index] !== undefined) this.variables.set(key, args[index]);
    }
    this.callDepth += 1;
    this.handlerStack.push(name);
    const outerReturning = this.returning;
    const outerReturnValue = this.returnValue;
    this.returning = false;
    this.returnValue = undefined;
    try {
      this.executeStatements(body, args);
      return this.returnValue;
    } finally {
      this.returning = outerReturning;
      this.returnValue = outerReturnValue;
      this.handlerStack.pop();
      this.callDepth -= 1;
    }
  }

  extractFunctionBody(script, name) {
    const escaped = name.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");
    const re = new RegExp(`(?:^|\\n)\\s*(?:(?:void|int|CString|float|double|char|long|UINT)\\s+)?${escaped}\\s*\\([^)]*\\)\\s*\\{`, "m");
    const match = re.exec(script);
    if (!match) return "";
    let depth = 1;
    let cursor = match.index + match[0].length;
    const start = cursor;
    let quote = "";
    let lineComment = false;
    let blockComment = false;
    while (cursor < script.length && depth > 0) {
      const ch = script[cursor++];
      const next = script[cursor] || "";
      if (lineComment) {
        if (ch === "\n" || ch === "\r") lineComment = false;
        continue;
      }
      if (blockComment) {
        if (ch === "*" && next === "/") {
          cursor += 1;
          blockComment = false;
        }
        continue;
      }
      if (quote) {
        if (ch === quote && script[cursor - 2] !== "\\") quote = "";
        continue;
      }
      if (ch === "/" && next === "/") {
        cursor += 1;
        lineComment = true;
        continue;
      }
      if (ch === "/" && next === "*") {
        cursor += 1;
        blockComment = true;
        continue;
      }
      if (ch === '"' || ch === "'") {
        quote = ch;
        continue;
      }
      if (ch === "{") depth += 1;
      if (ch === "}") depth -= 1;
    }
    return script.slice(start, cursor - 1);
  }

  executeStatements(body, args) {
    const cleaned = body
      .replace(/\/\/.*$/gm, "")
      .replace(/\/\*[\s\S]*?\*\//g, "");
    this.executeBlock(cleaned, args);
  }

  executeBlock(code, args) {
    let cursor = 0;
    while (cursor < code.length) {
      if (this.returning) return;
      cursor = this.skipSpace(code, cursor);
      if (cursor >= code.length) break;
      if (code[cursor] === "}") {
        cursor += 1;
        continue;
      }
      if (code[cursor] === "{") {
        const block = this.readBalanced(code, cursor, "{", "}");
        this.executeBlock(block.text, args);
        cursor = block.end;
        continue;
      }
      if (code.startsWith("if", cursor) && !/[A-Za-z0-9_]/.test(code[cursor + 2] || "")) {
        const conditionStart = this.skipSpace(code, cursor + 2);
        const condition = this.readBalanced(code, conditionStart, "(", ")");
        const trueUnit = this.readUnit(code, condition.end);
        let next = this.skipSpace(code, trueUnit.end);
        let falseUnit = { text: "", end: next };
        if (code.startsWith("else", next) && !/[A-Za-z0-9_]/.test(code[next + 4] || "")) {
          falseUnit = this.readUnit(code, next + 4);
          next = falseUnit.end;
        }
        this.executeBlock(this.evaluateCondition(condition.text) ? trueUnit.text : falseUnit.text, args);
        cursor = next;
        continue;
      }
      if (code.startsWith("while", cursor) && !/[A-Za-z0-9_]/.test(code[cursor + 5] || "")) {
        const conditionStart = this.skipSpace(code, cursor + 5);
        const condition = this.readBalanced(code, conditionStart, "(", ")");
        const unit = this.readUnit(code, condition.end);
        let guard = 0;
        while (this.evaluateCondition(condition.text) && guard < 256) {
          this.executeBlock(unit.text, args);
          guard += 1;
        }
        if (guard === 256) this.log("while loop limit reached");
        cursor = unit.end;
        continue;
      }
      if (code.startsWith("switch", cursor) && !/[A-Za-z0-9_]/.test(code[cursor + 6] || "")) {
        const conditionStart = this.skipSpace(code, cursor + 6);
        const condition = this.readBalanced(code, conditionStart, "(", ")");
        const unit = this.readUnit(code, condition.end);
        this.executeSwitch(condition.text, unit.text, args);
        cursor = unit.end;
        continue;
      }
      const statement = this.readStatement(code, cursor);
      this.executeStatement(statement.text.trim(), args);
      cursor = statement.end > cursor ? statement.end : cursor + 1;
    }
  }

  executeStatement(statement, args) {
    if (!statement || /^(switch|case|else|for|break)\b/.test(statement)) return;
    let match = /^return(?:\s+(.+))?$/.exec(statement);
    if (match) {
      this.returnValue = match[1] ? this.valueOf(match[1]) : undefined;
      this.returning = true;
      return;
    }
    match = /^(?:int|long|short|bool|float|double|char|CString|UINT)\s+([A-Za-z_][A-Za-z0-9_]*(?:\s*\[\s*\d+\s*\])?)$/.exec(statement);
    if (match) {
      const array = /^([A-Za-z_][A-Za-z0-9_]*)\s*\[\s*(\d+)\s*\]$/.exec(match[1]);
      if (array) {
        for (let index = 0; index < Number(array[2]); index += 1) this.variables.set(`${array[1]}[${index}]`, 0);
      } else if (!this.variables.has(match[1])) {
        this.variables.set(match[1], 0);
      }
      return;
    }
    match = /^(?:(?:int|long|short|bool|float|double|char|CString|UINT)\s+)?([A-Za-z_][A-Za-z0-9_]*(?:\s*\[[^\]]+\])?)\s*=\s*(.+)$/.exec(statement);
    if (match && !/^[=!<>]/.test(match[2])) {
      this.variables.set(this.variableKey(match[1]), this.valueOf(match[2]));
      return;
    }
    match = /^([A-Za-z_][A-Za-z0-9_]*(?:\s*\[[^\]]+\])?)\s*([+\-*/%])=\s*(.+)$/.exec(statement);
    if (match) {
      const key = this.variableKey(match[1]);
      const left = this.valueOf(key);
      const right = this.valueOf(match[3]);
      const op = match[2];
      this.variables.set(key, this.applyOperator(left, right, op));
      return;
    }
    match = /^([A-Za-z_][A-Za-z0-9_]*(?:\s*\[[^\]]+\])?)\s*(\+\+|--)$/.exec(statement);
    if (match) {
      const key = this.variableKey(match[1]);
      this.variables.set(key, this.valueOf(key) + (match[2] === "++" ? 1 : -1));
      return;
    }

    for (const call of this.findTopLevelCalls(statement)) {
      if (["if", "while", "switch", "for", "return"].includes(call.method)) continue;
      const values = this.parseArgs(call.args, args);
      this.dispatch(call.method, values);
    }
  }

  applyOperator(left, right, op) {
    const l = Number(left) || 0;
    const r = Number(right) || 0;
    switch (op) {
      case "+": return typeof left === "string" || typeof right === "string" ? `${left}${right}` : l + r;
      case "-": return l - r;
      case "*": return l * r;
      case "/": return r ? Math.trunc(l / r) : 0;
      case "%": return r ? l % r : 0;
      default: return 0;
    }
  }

  findTopLevelCalls(statement) {
    const calls = [];
    const re = /([A-Za-z_][A-Za-z0-9_.]*)\s*\(/g;
    let match;
    while ((match = re.exec(statement))) {
      const start = statement.indexOf("(", match.index);
      const balanced = this.readBalanced(statement, start, "(", ")");
      if (balanced.end > start) {
        calls.push({ method: match[1], args: balanced.text });
        re.lastIndex = balanced.end;
      }
    }
    return calls;
  }

  parseArgs(text) {
    if (!text.trim()) return [];
    return this.splitArgs(text).map((part) => {
      const token = part.trim();
      if (/^&\s*[A-Za-z_][A-Za-z0-9_]*(?:\s*\[[^\]]+\])?$/.test(token)) return { ref: this.variableKey(token.replace(/^&\s*/, "")) };
      return this.valueOf(token);
    });
  }

  splitArgs(text) {
    const parts = [];
    let start = 0;
    let depth = 0;
    let quote = "";
    for (let index = 0; index < text.length; index += 1) {
      const ch = text[index];
      if (quote) {
        if (ch === quote && text[index - 1] !== "\\") quote = "";
        continue;
      }
      if (ch === '"' || ch === "'") {
        quote = ch;
        continue;
      }
      if (ch === "(" || ch === "[") depth += 1;
      if (ch === ")" || ch === "]") depth -= 1;
      if (ch === "," && depth === 0) {
        parts.push(text.slice(start, index));
        start = index + 1;
      }
    }
    parts.push(text.slice(start));
    return parts;
  }

  skipSpace(code, cursor) {
    while (cursor < code.length && /\s/.test(code[cursor])) cursor += 1;
    return cursor;
  }

  readBalanced(code, cursor, open, close) {
    cursor = this.skipSpace(code, cursor);
    if (code[cursor] !== open) return { text: "", end: cursor };
    let depth = 1;
    let index = cursor + 1;
    const start = index;
    let quote = "";
    while (index < code.length && depth > 0) {
      const ch = code[index];
      if (quote) {
        if (ch === quote && code[index - 1] !== "\\") quote = "";
        index += 1;
        continue;
      }
      if (ch === '"' || ch === "'") {
        quote = ch;
        index += 1;
        continue;
      }
      if (ch === open) depth += 1;
      if (ch === close) depth -= 1;
      index += 1;
    }
    return { text: code.slice(start, index - 1), end: index };
  }

  readUnit(code, cursor) {
    cursor = this.skipSpace(code, cursor);
    if (code[cursor] === "{") return this.readBalanced(code, cursor, "{", "}");
    return this.readStatement(code, cursor);
  }

  readStatement(code, cursor) {
    let index = cursor;
    let paren = 0;
    let quote = "";
    while (index < code.length) {
      const ch = code[index];
      if (quote) {
        if (ch === quote && code[index - 1] !== "\\") quote = "";
        index += 1;
        continue;
      }
      if (ch === '"' || ch === "'") {
        quote = ch;
        index += 1;
        continue;
      }
      if (ch === "(") paren += 1;
      if (ch === ")") paren -= 1;
      if (ch === ";" && paren === 0) return { text: code.slice(cursor, index), end: index + 1 };
      if (ch === "}" && paren === 0) return { text: code.slice(cursor, index), end: index };
      index += 1;
    }
    return { text: code.slice(cursor), end: code.length };
  }

  readSwitchCases(text) {
    const cases = [];
    let index = 0;
    let depth = 0;
    let quote = "";
    let lineComment = false;
    let blockComment = false;
    while (index < text.length) {
      const ch = text[index];
      const next = text[index + 1] || "";
      if (lineComment) {
        if (ch === "\n" || ch === "\r") lineComment = false;
        index += 1;
        continue;
      }
      if (blockComment) {
        if (ch === "*" && next === "/") {
          index += 2;
          blockComment = false;
          continue;
        }
        index += 1;
        continue;
      }
      if (quote) {
        if (ch === quote && text[index - 1] !== "\\") quote = "";
        index += 1;
        continue;
      }
      if (ch === "/" && next === "/") {
        lineComment = true;
        index += 2;
        continue;
      }
      if (ch === "/" && next === "*") {
        blockComment = true;
        index += 2;
        continue;
      }
      if (ch === '"' || ch === "'") {
        quote = ch;
        index += 1;
        continue;
      }
      if (ch === "{") depth += 1;
      if (ch === "}") depth = Math.max(0, depth - 1);
      if (depth === 0) {
        const caseMatch = /^(case\s+([^:]+)|default)\s*:/.exec(text.slice(index));
        if (caseMatch) {
          cases.push({
            label: caseMatch[1] === "default" ? "default" : this.valueOf(caseMatch[2]),
            start: index + caseMatch[0].length,
            text: "",
          });
          index += caseMatch[0].length;
          continue;
        }
      }
      index += 1;
    }
    for (let cursor = 0; cursor < cases.length; cursor += 1) {
      const end = cursor + 1 < cases.length ? cases[cursor + 1].start - String(cases[cursor + 1].label).length : text.length;
      const nextCase = cursor + 1 < cases.length ? text.lastIndexOf("case", cases[cursor + 1].start) : -1;
      cases[cursor].text = text.slice(cases[cursor].start, nextCase >= 0 ? nextCase : end);
    }
    return cases;
  }

  findTopLevelBreak(text) {
    let depth = 0;
    let quote = "";
    for (let index = 0; index < text.length; index += 1) {
      const ch = text[index];
      if (quote) {
        if (ch === quote && text[index - 1] !== "\\") quote = "";
        continue;
      }
      if (ch === '"' || ch === "'") {
        quote = ch;
        continue;
      }
      if (ch === "{") depth += 1;
      if (ch === "}") depth = Math.max(0, depth - 1);
      if (depth === 0 && /\bbreak\s*;/.test(text.slice(index, index + 16))) return index;
    }
    return -1;
  }

  executeSwitch(condition, unit, args) {
    const value = this.valueOf(condition);
    const cases = this.readSwitchCases(unit);
    let index = cases.findIndex((item) => item.label === value);
    if (index < 0) index = cases.findIndex((item) => item.label === "default");
    if (index < 0) return;
    let body = "";
    for (let cursor = index; cursor < cases.length; cursor += 1) {
      const segment = cases[cursor].text;
      const breakAt = this.findTopLevelBreak(segment);
      if (breakAt >= 0) {
        body += segment.slice(0, breakAt);
        break;
      }
      body += segment;
    }
    this.executeBlock(body, args);
  }

  variableKey(expr) {
    const token = String(expr || "").trim();
    const match = /^([A-Za-z_][A-Za-z0-9_]*)\s*\[(.+)\]$/.exec(token);
    if (!match) return token;
    return `${match[1]}[${this.valueOf(match[2])}]`;
  }

  stripOuterParens(text) {
    let token = String(text || "").trim();
    while (token.startsWith("(") && token.endsWith(")")) {
      const balanced = this.readBalanced(token, 0, "(", ")");
      if (balanced.end !== token.length) break;
      token = balanced.text.trim();
    }
    return token;
  }

  findTopLevelOperator(text, operators) {
    let depth = 0;
    let quote = "";
    for (let index = text.length - 1; index >= 0; index -= 1) {
      const ch = text[index];
      if (quote) {
        if (ch === quote && text[index - 1] !== "\\") quote = "";
        continue;
      }
      if (ch === '"' || ch === "'") {
        quote = ch;
        continue;
      }
      if (ch === ")" || ch === "]") depth += 1;
      if (ch === "(" || ch === "[") depth -= 1;
      if (depth === 0 && operators.includes(ch)) {
        if ((ch === "+" || ch === "-") && (index === 0 || /[+\-*/%(<>!=]/.test(text[index - 1]))) continue;
        return { index, op: ch };
      }
    }
    return null;
  }

  valueOf(expr) {
    let token = this.stripOuterParens(expr);
    if (!token) return 0;
    if (/^-?\d+$/.test(token)) return Number(token);
    if (/^".*"$/.test(token) || /^'.*'$/.test(token)) return token.slice(1, -1);
    if (token.startsWith("!")) return this.evaluateCondition(token) ? 1 : 0;
    if (token.startsWith("-") && !/^-?\d+$/.test(token)) return -Number(this.valueOf(token.slice(1)));

    for (const ops of [["+", "-"], ["*", "/", "%"]]) {
      const found = this.findTopLevelOperator(token, ops);
      if (found) return this.applyOperator(this.valueOf(token.slice(0, found.index)), this.valueOf(token.slice(found.index + 1)), found.op);
    }

    const call = /^([A-Za-z_][A-Za-z0-9_.]*)\s*\(([\s\S]*)\)$/.exec(token);
    if (call) return this.callFunctionValue(call[1], this.parseArgs(call[2]));

    const key = this.variableKey(token);
    if (this.variables.has(key)) return this.variables.get(key);
    return 0;
  }

  callFunctionValue(method, args) {
    if (method === "Random") {
      const min = Number(args[0]) || 0;
      const max = Number(args[1]) || min;
      return min + Math.floor(Math.random() * (Math.max(0, max - min) + 1));
    }
    if (method.endsWith(".GetString")) {
      const objectName = method.replace(/\.GetString$/, "");
      return this.variables.get(objectName) ?? "";
    }
    if (method === "Recorder.IsEmpty") return this.recorder.hasContent ? 0 : 1;
    if (method === "IsProject") return 0;
    if (!method.includes(".") && this.extractFunctionBody(this.scene?.script?.rawText || "", method)) {
      const result = this.executeHandler(method, args);
      return result ?? 0;
    }
    return 0;
  }

  evaluateCondition(condition) {
    let text = this.stripOuterParens(condition);
    if (!text) return false;
    const orParts = this.splitTopLevel(text, "||");
    if (orParts.length > 1) return orParts.some((part) => this.evaluateCondition(part));
    const andParts = this.splitTopLevel(text, "&&");
    if (andParts.length > 1) return andParts.every((part) => this.evaluateCondition(part));
    const compare = this.findTopLevelCompare(text);
    if (compare) {
      const left = this.valueOf(text.slice(0, compare.index));
      const right = this.valueOf(text.slice(compare.index + compare.op.length));
      switch (compare.op) {
        case "==": return left === right;
        case "!=": return left !== right;
        case ">=": return Number(left) >= Number(right);
        case "<=": return Number(left) <= Number(right);
        case ">": return Number(left) > Number(right);
        case "<": return Number(left) < Number(right);
      }
    }
    if (text.startsWith("!")) return !this.evaluateCondition(text.slice(1));
    return Boolean(this.valueOf(text));
  }

  splitTopLevel(text, separator) {
    const parts = [];
    let start = 0;
    let depth = 0;
    let quote = "";
    for (let index = 0; index < text.length; index += 1) {
      const ch = text[index];
      if (quote) {
        if (ch === quote && text[index - 1] !== "\\") quote = "";
        continue;
      }
      if (ch === '"' || ch === "'") {
        quote = ch;
        continue;
      }
      if (ch === "(" || ch === "[") depth += 1;
      if (ch === ")" || ch === "]") depth -= 1;
      if (depth === 0 && text.startsWith(separator, index)) {
        parts.push(text.slice(start, index));
        start = index + separator.length;
        index += separator.length - 1;
      }
    }
    parts.push(text.slice(start));
    return parts.map((part) => part.trim()).filter(Boolean);
  }

  findTopLevelCompare(text) {
    const ops = ["==", "!=", ">=", "<=", ">", "<"];
    let depth = 0;
    let quote = "";
    for (let index = 0; index < text.length; index += 1) {
      const ch = text[index];
      if (quote) {
        if (ch === quote && text[index - 1] !== "\\") quote = "";
        continue;
      }
      if (ch === '"' || ch === "'") {
        quote = ch;
        continue;
      }
      if (ch === "(" || ch === "[") depth += 1;
      if (ch === ")" || ch === "]") depth -= 1;
      if (depth === 0) {
        for (const op of ops) {
          if (text.startsWith(op, index)) return { index, op };
        }
      }
    }
    return null;
  }

  setRef(arg, value) {
    if (arg && typeof arg === "object" && arg.ref) this.variables.set(arg.ref, value);
  }

  loadGroup(name) {
    const clean = this.groupKey(name);
    const group = this.groups.get(clean);
    this.closeGroup();
    this.loadedGroup = clean;
    if (!group) {
      this.log(`LoadGroup(${name}) missing exported group`);
      return;
    }
    this.group = group;
    for (const component of group.components || []) {
      for (const asset of component.assets || []) {
        this.indexAsset("Group", component, asset);
        if (!asset.url) continue;
        const img = document.createElement("img");
        img.className = "layer group-layer runtime-hidden";
        img.src = component.type === "Transparent_Video_Holder" && asset.stillUrl ? asset.stillUrl : asset.url;
        img.alt = `${component.id}:${asset.id ?? ""}`;
        img.dataset.runtimeLayer = "1";
        img.dataset.originalSrc = asset.url;
        if (asset.stillUrl) img.dataset.stillSrc = asset.stillUrl;
        img.dataset.componentType = component.type;
        img.dataset.runtimeNamespace = "Group";
        img.dataset.assetId = String(asset.id ?? "");
        img.dataset.runtimeId = String(asset.id ?? 0);
        const rect = asset.rect || component.rect || { x: 0, y: 0, width: asset.width || 1, height: asset.height || 1 };
        img.style.left = `${rect.x}px`;
        img.style.top = `${rect.y}px`;
        img.style.width = `${asset.width || rect.width}px`;
        img.style.height = `${asset.height || rect.height}px`;
        img.style.zIndex = String(600000 + Number(asset.z ?? component.z ?? 0));
        this.bindRuntimeEvents(img, component.type, Number(asset.id ?? 0), "Group");
        this.stage.appendChild(img);
        this.indexLayer(img);
      }
      for (const asset of component.audio || []) {
        if (!asset.url) continue;
        const audio = new Audio(asset.url);
        audio.preload = "none";
        audio.dataset.componentType = component.type;
        audio.dataset.runtimeNamespace = "Group";
        audio.dataset.assetId = String(asset.id ?? "");
        audio.dataset.runtimeId = String(asset.id ?? 0);
        audio.dataset.durationSeconds = String(asset.durationSeconds || "");
        this.indexAudio(audio);
      }
      for (const hotspot of component.hitboxes || []) {
        const button = document.createElement("button");
        button.type = "button";
        button.className = "hitbox group-layer";
        button.dataset.componentType = "HotSpot_Holder";
        button.dataset.runtimeNamespace = "Group";
        button.dataset.runtimeId = String(hotspot.id);
        const rect = { ...(hotspot.rect || { x: 0, y: 0, width: 1, height: 1 }) };
        const stageWidth = parseInt(this.stage.style.width || "640", 10) || 640;
        const stageHeight = parseInt(this.stage.style.height || "480", 10) || 480;
        if (Number(hotspot.id) === 0 && rect.x >= stageWidth - 2) {
          rect.x = Math.max(0, stageWidth - 28);
          rect.y = Math.max(0, rect.y);
          rect.width = 28;
          rect.height = Math.max(stageHeight, rect.height || 1);
        }
        if (Number(hotspot.id) === 1 && rect.width > stageWidth * 0.75) {
          rect.x = Math.max(0, rect.x);
          rect.width = Math.max(1, stageWidth - 170 - rect.x);
        }
        button.style.left = `${rect.x}px`;
        button.style.top = `${rect.y}px`;
        button.style.width = `${Math.max(8, rect.width)}px`;
        button.style.height = `${Math.max(8, rect.height)}px`;
        button.style.zIndex = String(700000 + Number(hotspot.id || 0));
        button.title = `Group HotSpot ${hotspot.id}`;
        this.bindRuntimeEvents(button, "HotSpot_Holder", Number(hotspot.id), "Group");
        this.stage.appendChild(button);
      }
      if (component.type === "Sprite_Holder" && Number(component.itemCount) > 0) {
        for (let id = 0; id < Number(component.itemCount); id += 1) {
          const layer = this.createSyntheticLayer("Group", "Sprite_Holder", id, component.rect);
          layer.classList.add("group-layer");
        }
      }
    }
    this.log(`LoadGroup(${name}) mounted ${group.components?.length || 0} component(s)`);
  }

  closeGroup() {
    for (const item of [...this.audio.values()]) {
      if (item.dataset.runtimeNamespace === "Group") {
        item.pause();
        item.currentTime = 0;
      }
    }
    for (const item of this.stage.querySelectorAll('[data-runtime-namespace="Group"], .group-layer')) item.remove();
    for (const map of [this.layers, this.audio, this.assets, this.hotspots]) {
      for (const key of [...map.keys()]) {
        if (String(key).startsWith("Group:")) map.delete(key);
      }
    }
    this.group = null;
  }

  showPuzzle(args = []) {
    this.puzzle.active = true;
    const layer = this.findLayer("Puzzle", 0, "Page", true);
    const asset = this.findAsset("Puzzle", 0, "Page");
    layer.className = "puzzle-surface";
    layer.dataset.runtimeLayer = "1";
    layer.dataset.componentType = "Puzzle";
    layer.dataset.runtimeNamespace = "Page";
    layer.dataset.runtimeId = "0";
    layer.style.left = `${Number(args[1] ?? 0) || 0}px`;
    layer.style.top = `${Number(args[2] ?? 0) || 0}px`;
    layer.style.width = `${Math.max(80, Number(args[3] ?? 580) || 580)}px`;
    layer.style.height = `${Math.max(80, Number(args[4] ?? 480) || 480)}px`;
    layer.style.zIndex = "500000";
    if (asset?.url) {
      layer.style.backgroundImage = `url("${asset.url}")`;
      layer.style.backgroundSize = "cover";
      layer.style.backgroundPosition = "center";
      let image = layer.querySelector?.("img.puzzle-image");
      if (!image) {
        image = document.createElement("img");
        image.className = "puzzle-image";
        image.alt = "";
        layer.appendChild(image);
      }
      image.src = asset.url;
    }
    layer.classList.remove("runtime-hidden");
    this.bindRuntimeEvents(layer, "Puzzle", 0, "Page");
    this.log(`Puzzle.Open/Mix(${args.join(",")})`);
  }

  setText(id, text = "") {
    const layer = this.findLayer("Text_Holder", id, "Page", true);
    layer.textContent = String(text || layer.textContent || "");
  }

  dispatch(method, args) {
    const isGroup = method.startsWith("Group.");
    const local = method.replace(/^Group\./, "");
    const namespace = isGroup ? "Group" : "Page";
    switch (local) {
      case "LoadGroup":
        this.loadGroup(String(args[0] || ""));
        break;
      case "CloseGroup":
        this.closeGroup();
        this.log("CloseGroup()");
        break;
      case "LoadPage":
        if (this.handlerStack.includes("Group.Sprite_Holder.InPlace") && this.trustedCallbackDepth <= 0) {
          this.log(`blocked untrusted ${namespace}.LoadPage(${args[0] || ""})`);
          break;
        }
        this.log(`LoadPage(${args[0] || ""})`);
        if (this.onLoadPage) this.onLoadPage(String(args[0] || ""));
        break;
      case "SetCursor":
        this.cursorId = Number(args[0]) || 0;
        this.stage.dataset.cursorId = String(this.cursorId);
        this.log(`SetCursor(${args.join(",")})`);
        break;
      case "FadeScreen":
      case "ShowBuffer":
        this.log(`${local}(${args.join(",")})`);
        break;
      case "SetTimer":
        this.clearTimers();
        if (Number(args[0]) > 0) {
          const delay = Math.max(1, Number(args[0]));
          this.timers.push(setTimeout(() => this.executeHandler("OnTimer"), delay));
        }
        this.log(`SetTimer(${args.join(",")})`);
        break;
      case "EnableScreen":
        this.screenEnabled = true;
        this.log("EnableScreen()");
        break;
      case "DisableScreen":
        this.screenEnabled = false;
        this.log("DisableScreen()");
        break;
      case "Sound_Holder.PlayDSound":
        this.playAudio("Sound_Holder", Number(args[0]), namespace);
        break;
      case "Sound_Holder.Stop":
        this.stopAudio("Sound_Holder", Number(args[0]), namespace);
        break;
      case "Sound_Holder.StopAll":
        this.stopAudio("Sound_Holder", undefined, namespace);
        break;
      case "Sound_Holder.SetPlayDSoundParameters":
      case "Text_Holder.SetSoundParameters":
        this.log(`${namespace}.${local}(${args.join(",")})`);
        break;
      case "Transparent_Video_Holder.Play":
      case "Video_Holder.Play":
      case "Video_Holder.PlayFromTo":
        this.playVideo(local.split(".")[0], Number(args[0]), namespace);
        break;
      case "Transparent_Video_Holder.Stop":
      case "Video_Holder.Stop":
        this.stopAudio(local.split(".")[0], undefined, namespace);
        this.clearVideoTimers();
        this.log(`${namespace}.${local}(${args.join(",")})`);
        break;
      case "Transparent_Video_Holder.ResetVideo":
        this.setLayerStill("Transparent_Video_Holder", Number(args[0]), true, namespace);
        break;
      case "Transparent_Video_Holder.ShowFirsLastVideoFrame":
        this.setLayerStill("Transparent_Video_Holder", Number(args[0]), false, namespace);
        break;
      case "Transparent_Video_Holder.HideFirsLastVideoFrame":
        this.setLayerVisible("Transparent_Video_Holder", Number(args[0]), false, namespace);
        break;
      case "Transparent_Video_Holder.SetDeep":
      case "Sprite_Holder.SetDeep":
        this.setDeep(local.split(".")[0], Number(args[0]), Number(args[1]), namespace);
        break;
      case "MultiBitmap.ShowBitmap":
      case "Bitmap_Holder.ShowBitmap":
        if (args.length >= 3) {
          this.moveLayer(local.split(".")[0], Number(args[0]), Number(args[1] || 0), Number(args[2] || 0), Number(args[3] || 0), namespace);
        } else {
          this.setLayerVisible(local.split(".")[0], Number(args[0]), true, namespace);
        }
        break;
      case "Bitmap_Holder.MoveTo":
      case "Sprite_Holder.GotoXY":
      case "Sprite_Holder.MoveTo":
      case "HotSpot_Holder.MoveTo":
        this.moveLayer(local.split(".")[0], Number(args[0]), Number(args[1] || 0), Number(args[2] || 0), undefined, namespace);
        break;
      case "MultiBitmap.HideBitmap":
      case "Bitmap_Holder.HideBitmap":
        this.setLayerVisible(local.split(".")[0], Number(args[0]), false, namespace);
        break;
      case "Bitmap_Holder.EnableDrag":
      case "Bitmap_Holder.DisableDrag":
      case "Sprite_Holder.CompEnableDrag":
      case "Sprite_Holder.CompDisableDrag":
      case "Sprite_Holder.DisableDragMode":
        this.log(`${namespace}.${local}(${args.join(",")})`);
        break;
      case "Sprite_Holder.ShowSprite":
        this.setLayerVisible("Sprite_Holder", Number(args[0]), true, namespace);
        break;
      case "Sprite_Holder.HideSprite":
        this.setLayerVisible("Sprite_Holder", Number(args[0]), false, namespace);
        break;
      case "Sprite_Holder.ChangePhase":
        this.changePhase("Sprite_Holder", Number(args[0]), Number(args[1] || 0), namespace);
        break;
      case "Sprite_Holder.GetPosition": {
        const layer = this.findLayer("Sprite_Holder", Number(args[0]), namespace, true);
        this.setRef(args[1], parseInt(layer.style.left || "0", 10) || 0);
        this.setRef(args[2], parseInt(layer.style.top || "0", 10) || 0);
        this.setRef(args[3], parseInt(layer.style.zIndex || "0", 10) || 0);
        break;
      }
      case "Sprite_Holder.EnableTimers":
      case "Sprite_Holder.DisableTimers":
      case "Sprite_Holder.SynchronizeTimers":
      case "Sprite_Holder.StopAnimation":
      case "Sprite_Holder.ContinueAnimation":
        this.log(`${namespace}.${local}(${args.join(",")})`);
        break;
      case "HotSpot_Holder.EnableHotSpot":
        this.enableHotspot(Number(args[0]), true, namespace);
        break;
      case "HotSpot_Holder.DisableHotSpot":
        this.enableHotspot(Number(args[0]), false, namespace);
        break;
      case "HotSpot_Holder.LeftButtonClickOn":
      case "HotSpot_Holder.LeftButtonClickOnUp":
        this.dispatchComponentEvent(
          "HotSpot_Holder",
          local.endsWith("Up") ? "MouseClickOnUp" : "MouseClickOnDown",
          Number(args[0]),
          [],
          namespace
        );
        break;
      case "HotSpot_Holder.PointInHotSpot":
        this.setRef(args[3], 1);
        break;
      case "Text_Holder.ShowText":
        this.setLayerVisible("Text_Holder", Number(args[0]), true, namespace);
        break;
      case "Text_Holder.HideText":
        this.setLayerVisible("Text_Holder", Number(args[0]), false, namespace);
        break;
      case "Text_Holder.SetText":
        this.setText(Number(args[0]), args[1]);
        break;
      case "Text_Holder.PlaySynchroText":
        this.setLayerVisible("Text_Holder", Number(args[0]), true, namespace);
        this.playAudio("Text_Holder", Number(args[0]), namespace);
        this.timers.push(setTimeout(() => this.executeHandler("Text_Holder.EndOfSynchroText", [Number(args[0])]), 1800));
        break;
      case "Text_Holder.StopSynchroText":
        this.stopAudio("Text_Holder", Number(args[0]), namespace);
        break;
      case "Text_Holder.SetTextOffsets":
      case "Text_Holder.EnableMouse":
      case "Text_Holder.DisableMouse":
      case "Text_Holder.SetCurrentSelect":
        this.log(`${namespace}.${local}(${args.join(",")})`);
        break;
      case "Text_Holder.GetTextOffsets":
        this.setRef(args[1], 0);
        this.setRef(args[2], 0);
        break;
      case "Puzzle.OpenPuzzle":
      case "Puzzle.Mix":
        this.showPuzzle(args);
        break;
      case "Puzzle.ShowFullBitmap":
        this.showPuzzle([0, Number(args[0] || 0), Number(args[1] || 0), 460, 360]);
        this.timers.push(setTimeout(() => this.executeHandler("Puzzle.EndShowFullBitmap", [Number(args[2] || 0)]), 500));
        break;
      case "Puzzle.EndShowFullBitmap":
      case "Puzzle.EndShowBitmap":
        this.executeHandler(local, args);
        break;
      case "Recorder.OpenFile":
        this.recorder.file = String(args[0] || "");
        this.log(`Recorder.OpenFile(${this.recorder.file})`);
        break;
      case "Recorder.CloseFile":
        this.recorder.file = "";
        this.log("Recorder.CloseFile()");
        break;
      case "Recorder.EmptyFile":
        this.recorder.hasContent = false;
        this.log("Recorder.EmptyFile()");
        break;
      case "Recorder.IsEmpty":
        this.setRef(args[0], this.recorder.hasContent ? 0 : 1);
        this.log("Recorder.IsEmpty()");
        break;
      case "Recorder.Record":
        this.recorder.recording = true;
        this.recorder.hasContent = true;
        this.timers.push(setTimeout(() => this.executeHandler("Recorder.EndRecordSound"), 1000));
        this.log("Recorder.Record()");
        break;
      case "Recorder.Stop":
        this.recorder.recording = false;
        this.recorder.playing = false;
        this.log("Recorder.Stop()");
        break;
      case "Recorder.Play":
        this.recorder.playing = true;
        this.timers.push(setTimeout(() => this.executeHandler("Recorder.EndPlaySound"), 1000));
        this.log("Recorder.Play()");
        break;
      default:
        if (/^[A-Za-z_][A-Za-z0-9_]*\.SetString$/.test(method)) {
          this.variables.set(method.replace(/\.SetString$/, ""), args[0] ?? "");
        } else if (/^[A-Za-z_][A-Za-z0-9_]*\.Format$/.test(method)) {
          const objectName = method.replace(/\.Format$/, "");
          let index = 1;
          const formatted = String(args[0] || "").replace(/%[sd]/g, () => String(args[index++] ?? ""));
          this.variables.set(objectName, formatted);
        } else if (/^[A-Za-z_][A-Za-z0-9_]*\.SetRect$/.test(method) || method === "SetRect") {
          this.log(`${method}(${args.join(",")})`);
        } else if (!method.includes(".") && this.extractFunctionBody(this.scene?.script?.rawText || "", method)) {
          this.executeHandler(method, args);
        } else if (method.includes(".")) {
          this.log(`stub ${method}(${args.join(",")})`);
        }
        break;
    }
  }
}
