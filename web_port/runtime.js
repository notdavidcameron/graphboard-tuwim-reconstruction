class GraphBoardRuntime {
  constructor(stage, logTarget) {
    this.stage = stage;
    this.logTarget = logTarget;
    this.scene = null;
    this.layers = new Map();
    this.audio = new Map();
    this.assets = new Map();
    this.hotspots = new Map();
    this.variables = new Map();
    this.loadedGroup = null;
    this.screenEnabled = true;
    this.callDepth = 0;
    this.onLoadPage = null;
    this.timers = [];
    this.videoTimers = new Map();
    this.returning = false;
  }

  reset(scene) {
    this.clearTimers();
    this.clearVideoTimers();
    this.scene = scene;
    this.layers.clear();
    this.audio.clear();
    this.assets.clear();
    this.hotspots.clear();
    this.variables.clear();
    this.loadedGroup = null;
    this.screenEnabled = true;
    if (this.logTarget) this.logTarget.textContent = "";
    this.indexSceneAssets();
    this.indexExistingDom();
    this.log(`runtime ready: ${scene?.id || "no scene"}`);
  }

  indexSceneAssets() {
    for (const component of this.scene?.components || []) {
      for (const asset of component.assets || []) {
        const id = Number(asset.id ?? 0);
        this.assets.set(`${component.type}:${id}`, asset);
        const runtimeId = (component.type === "Transparent_Video_Holder" || component.type === "Sprite_Holder") && id > 0 && id % 2 === 0
          ? id / 2
          : id;
        this.assets.set(`${component.type}:${runtimeId}`, asset);
      }
    }
  }

  indexExistingDom() {
    for (const layer of this.stage.querySelectorAll(".layer")) {
      const key = layer.dataset.runtimeKey;
      if (key) this.layers.set(key, layer);
    }
    if (typeof document === "undefined") return;
    for (const audio of document.querySelectorAll("#audioList audio")) {
      const key = audio.dataset.runtimeKey;
      if (key) this.audio.set(key, audio);
      if (!audio.dataset.runtimeIndexed) {
        audio.dataset.runtimeIndexed = "1";
        audio.addEventListener("ended", () => {
          const type = audio.dataset.componentType;
          const id = Number(audio.dataset.runtimeId ?? audio.dataset.assetId ?? 0);
          this.executeHandler(`${type}.EndPlaySound`, [id]);
          if (type === "Sound_Holder") this.executeHandler("Sound_Holder.EndPlaySound", [id]);
        });
      }
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
    this.logTarget.textContent = `${line}\n${this.logTarget.textContent}`.slice(0, 7000);
  }

  component(type) {
    return (this.scene?.components || []).find((component) => component.type === type);
  }

  componentAssets(type) {
    const component = this.component(type);
    return component?.assets || [];
  }

  findLayer(type, id) {
    return this.layers.get(`${type}:${id}`) || this.layers.get(`${type}:${id * 2}`) || this.layers.get(`${type}:${Math.max(0, id - 1) * 2}`) || null;
  }

  findAsset(type, id) {
    return this.assets.get(`${type}:${id}`) || this.assets.get(`${type}:${id * 2}`) || this.assets.get(`${type}:${Math.max(0, id - 1) * 2}`) || null;
  }

  findAudio(type, id) {
    return this.audio.get(`${type}:${id}`) || this.audio.get(`${type}:${id * 2}`) || this.audio.get(`${type}:${Math.max(0, id - 1) * 2}`) || null;
  }

  setLayerVisible(type, id, visible) {
    const layer = this.findLayer(type, id);
    if (!layer) {
      this.log(`${type}.${visible ? "show" : "hide"}(${id}) missing layer`);
      return;
    }
    layer.classList.toggle("runtime-hidden", !visible);
    this.log(`${type}.${visible ? "show" : "hide"}(${id})`);
  }

  setLayerStill(type, id, first = true) {
    const layer = this.findLayer(type, id);
    if (!layer) return;
    layer.dataset.videoState = first ? "first" : "last";
    layer.classList.remove("runtime-playing");
    layer.classList.toggle("runtime-hidden", false);
    if (first && layer.dataset.originalSrc) {
      layer.src = layer.dataset.originalSrc;
      setTimeout(() => {
        if (layer.dataset.videoState === "first") this.freezeLayer(layer, "first");
      }, 80);
    }
  }

  restartLayerAnimation(layer) {
    if (!layer) return;
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
    if (!layer || !layer.complete || layer.naturalWidth <= 0 || layer.naturalHeight <= 0) return;
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

  moveLayer(type, id, x, y, z = undefined) {
    const layer = this.findLayer(type, id);
    if (!layer) {
      this.log(`${type}.move(${id},${x},${y}) missing layer`);
      return;
    }
    layer.style.left = `${x}px`;
    layer.style.top = `${y}px`;
    layer.style.transition = "left 180ms linear, top 180ms linear";
    if (Number.isFinite(z)) layer.style.zIndex = String(10 + z);
    layer.classList.remove("runtime-hidden");
    this.log(`${type}.move(${id},${x},${y}${Number.isFinite(z) ? `,${z}` : ""})`);
  }

  playAudio(type, id) {
    const audio = this.findAudio(type, id);
    if (!audio) {
      this.log(`${type}.PlayDSound(${id}) missing audio`);
      return;
    }
    for (const item of this.audio.values()) {
      if (item !== audio && item.dataset.componentType === type) item.pause();
    }
    audio.currentTime = 0;
    audio.play().catch((error) => this.log(`audio blocked: ${error.message}`));
    this.log(`${type}.PlayDSound(${id})`);
  }

  stopAudio(type, id) {
    const audio = Number.isFinite(id) ? this.findAudio(type, id) : null;
    const targets = audio ? [audio] : [...this.audio.values()].filter((item) => item.dataset.componentType === type);
    for (const item of targets) {
      item.pause();
      item.currentTime = 0;
    }
    this.log(`${type}.Stop(${Number.isFinite(id) ? id : "all"})`);
  }

  videoDurationMs(type, id) {
    const asset = this.findAsset(type, id);
    const frameDuration = Number(asset?.frameCount || 1) * 100;
    const matchingAudio = this.findAudio(type, id);
    const audioDuration = Number(matchingAudio?.dataset.durationSeconds || 0) * 1000;
    return Math.max(250, audioDuration || frameDuration);
  }

  playVideo(type, id) {
    const layer = this.findLayer(type, id);
    if (!layer) {
      this.log(`${type}.Play(${id}) missing layer`);
      return;
    }
    const key = `${type}:${id}`;
    if (this.videoTimers.has(key)) clearTimeout(this.videoTimers.get(key));
    this.restartLayerAnimation(layer);
    this.playAudio(type, id);
    const delay = this.videoDurationMs(type, id);
    this.videoTimers.set(key, setTimeout(() => {
      this.videoTimers.delete(key);
      layer.classList.remove("runtime-playing");
      this.freezeLayer(layer, "last");
      this.executeHandler(`${type}.TheEnd`, [id]);
    }, delay));
    this.log(`${type}.Play(${id}) for ${Math.round(delay)}ms`);
  }

  enableHotspot(id, enabled) {
    this.hotspots.set(id, enabled);
    for (const target of this.stage.querySelectorAll(`[data-component-type="HotSpot_Holder"][data-runtime-id="${id}"]`)) {
      target.classList.toggle("runtime-disabled", !enabled);
    }
    this.log(`HotSpot ${id} ${enabled ? "enabled" : "disabled"}`);
  }

  dispatchComponentEvent(type, eventName, id, extra = []) {
    if (type === "HotSpot_Holder" && this.hotspots.has(id) && !this.hotspots.get(id)) {
      this.log(`HotSpot ${id} ignored (${eventName})`);
      return;
    }
    const names = [
      `${type}.${eventName}`,
      `Group.${type}.${eventName}`,
    ];
    for (const name of names) {
      if (this.extractFunctionBody(this.scene?.script?.rawText || "", name)) {
        this.executeHandler(name, [id, ...extra]);
        return;
      }
    }
    this.log(`event ${type}.${eventName}(${[id, ...extra].join(",")})`);
  }

  executeHandler(name, args = []) {
    const script = this.scene?.script?.rawText || "";
    const body = this.extractFunctionBody(script, name);
    if (!body) {
      this.log(`handler not found: ${name}`);
      return;
    }
    if (this.callDepth > 12) {
      this.log(`handler recursion limit: ${name}`);
      return;
    }
    this.log(`run ${name}(${args.join(",")})`);
    for (const [key, index] of Object.entries({ rectID: 0, videoID: 0, spriteID: 0, soundID: 0, textID: 0, x: 1, y: 2 })) {
      if (args[index] !== undefined) this.variables.set(key, args[index]);
    }
    this.callDepth += 1;
    const outerReturning = this.returning;
    this.returning = false;
    try {
      this.executeStatements(body, args);
    } finally {
      this.returning = outerReturning;
      this.callDepth -= 1;
    }
  }

  extractFunctionBody(script, name) {
    const escaped = name.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");
    const re = new RegExp(`(?:^|\\n)\\s*(?:(?:void|int|CString|float|double|char|long)\\s+)?${escaped}\\s*\\([^)]*\\)\\s*\\{`, "m");
    const match = re.exec(script);
    if (!match) return "";
    let depth = 1;
    let cursor = match.index + match[0].length;
    const start = cursor;
    while (cursor < script.length && depth > 0) {
      const ch = script[cursor++];
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
        while (this.evaluateCondition(condition.text) && guard < 128) {
          this.executeBlock(unit.text, args);
          guard += 1;
        }
        if (guard === 128) this.log("while loop limit reached");
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
      cursor = statement.end;
    }
  }

  executeStatement(statement, args) {
    if (!statement || /^(switch|case|else|for|break)\b/.test(statement)) return;
    if (/^return\b/.test(statement)) {
      this.returning = true;
      return;
    }

    let match = /^(?:int|long|short|bool|float|double|char|CString)?\s*([A-Za-z_][A-Za-z0-9_]*(?:\[[^\]]+\])?)\s*=\s*(.+)$/.exec(statement);
    if (match) {
      this.variables.set(this.variableKey(match[1]), this.valueOf(match[2].trim()));
      return;
    }
    match = /^([A-Za-z_][A-Za-z0-9_]*(?:\[[^\]]+\])?)\s*(\+\+|--)$/.exec(statement);
    if (match) {
      const key = this.variableKey(match[1]);
      this.variables.set(key, this.valueOf(key) + (match[2] === "++" ? 1 : -1));
      return;
    }

    const calls = [...statement.matchAll(/([A-Za-z_][A-Za-z0-9_.]*)\s*\(([^()]*)\)/g)];
    if (calls.length === 0) return;
    for (const call of calls) {
      const method = call[1];
      if (["if", "while", "switch", "for", "return"].includes(method)) continue;
      const values = this.parseArgs(call[2], args);
      this.dispatch(method, values);
    }
  }

  parseArgs(text, handlerArgs) {
    if (!text.trim()) return [];
    return text.split(",").map((part) => {
      const token = part.trim();
      if (/^-?\d+$/.test(token)) return Number(token);
      if (/^".*"$/.test(token)) return token.slice(1, -1);
      if (token === "rectID") return handlerArgs[0] ?? 0;
      if (token === "videoID") return handlerArgs[0] ?? 0;
      if (token === "spriteID") return handlerArgs[0] ?? 0;
      if (token === "x") return handlerArgs[1] ?? 0;
      if (token === "y") return handlerArgs[2] ?? 0;
      if (this.variables.has(this.variableKey(token))) return this.variables.get(this.variableKey(token));
      const simpleMath = /^([A-Za-z_][A-Za-z0-9_]*)\s*([+-])\s*(\d+)$/.exec(token);
      if (simpleMath && this.variables.has(simpleMath[1])) {
        return this.variables.get(simpleMath[1]) + (simpleMath[2] === "+" ? 1 : -1) * Number(simpleMath[3]);
      }
      return token;
    });
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
    while (index < code.length && depth > 0) {
      if (code[index] === open) depth += 1;
      if (code[index] === close) depth -= 1;
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
    while (index < code.length) {
      const ch = code[index];
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
    const re = /\b(case\s+([^:]+)|default)\s*:/g;
    let match = re.exec(text);
    while (match) {
      const start = re.lastIndex;
      const label = match[1] === "default" ? "default" : this.valueOf(match[2]);
      const next = re.exec(text);
      const end = next ? next.index : text.length;
      cases.push({ label, text: text.slice(start, end) });
      match = next;
    }
    return cases;
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
      const breakAt = segment.search(/\bbreak\s*;/);
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

  valueOf(expr) {
    const token = String(expr || "").trim();
    if (/^-?\d+$/.test(token)) return Number(token);
    const key = this.variableKey(token);
    if (this.variables.has(key)) return this.variables.get(key);
    const math = /^(.+?)\s*([+-])\s*(-?\d+)$/.exec(token);
    if (math) return this.valueOf(math[1]) + (math[2] === "+" ? 1 : -1) * Number(math[3]);
    return 0;
  }

  evaluateCondition(condition) {
    let text = String(condition || "").trim();
    if (!text) return false;
    text = text.replace(/^\((.*)\)$/, "$1").trim();
    const orParts = text.split("||");
    if (orParts.length > 1) return orParts.some((part) => this.evaluateCondition(part));
    const andParts = text.split("&&");
    if (andParts.length > 1) return andParts.every((part) => this.evaluateCondition(part));
    const compare = /^(.+?)\s*(==|!=|>=|<=|>|<)\s*(.+)$/.exec(text);
    if (compare) {
      const left = this.valueOf(compare[1]);
      const right = this.valueOf(compare[3]);
      switch (compare[2]) {
        case "==": return left === right;
        case "!=": return left !== right;
        case ">=": return left >= right;
        case "<=": return left <= right;
        case ">": return left > right;
        case "<": return left < right;
      }
    }
    if (text.startsWith("!")) return !this.evaluateCondition(text.slice(1));
    return Boolean(this.valueOf(text));
  }

  dispatch(method, args) {
    const local = method.replace(/^Group\./, "");
    switch (local) {
      case "LoadGroup":
        this.loadedGroup = String(args[0] || "");
        this.log(`LoadGroup(${this.loadedGroup})`);
        break;
      case "LoadPage":
        this.log(`LoadPage(${args[0] || ""})`);
        if (this.onLoadPage) this.onLoadPage(String(args[0] || ""));
        break;
      case "SetCursor":
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
        this.playAudio("Sound_Holder", Number(args[0]));
        break;
      case "Sound_Holder.Stop":
        this.stopAudio("Sound_Holder", Number(args[0]));
        break;
      case "Sound_Holder.StopAll":
        this.stopAudio("Sound_Holder");
        break;
      case "Sound_Holder.SetPlayDSoundParameters":
        this.log(`${local}(${args.join(",")})`);
        break;
      case "Transparent_Video_Holder.Play":
        this.playVideo("Transparent_Video_Holder", Number(args[0]));
        break;
      case "Transparent_Video_Holder.Stop":
        this.stopAudio("Transparent_Video_Holder");
        this.clearVideoTimers();
        this.log("Transparent_Video_Holder.Stop()");
        break;
      case "Transparent_Video_Holder.ResetVideo":
        this.setLayerStill("Transparent_Video_Holder", Number(args[0]), true);
        break;
      case "Transparent_Video_Holder.ShowFirsLastVideoFrame":
        this.setLayerStill("Transparent_Video_Holder", Number(args[0]), false);
        break;
      case "Transparent_Video_Holder.HideFirsLastVideoFrame":
        this.setLayerVisible("Transparent_Video_Holder", Number(args[0]), false);
        break;
      case "Transparent_Video_Holder.SetDeep":
      case "Transparent_Video_Holder.GetDeep":
        this.log(`${local}(${args.join(",")})`);
        break;
      case "MultiBitmap.ShowBitmap":
      case "Bitmap_Holder.ShowBitmap":
        this.moveLayer(local.split(".")[0], Number(args[0]), Number(args[1] || 0), Number(args[2] || 0), Number(args[3] || 0));
        break;
      case "MultiBitmap.HideBitmap":
      case "Bitmap_Holder.HideBitmap":
        this.setLayerVisible(local.split(".")[0], Number(args[0]), false);
        break;
      case "Sprite_Holder.GotoXY":
      case "Sprite_Holder.MoveTo":
        this.moveLayer("Sprite_Holder", Number(args[0]), Number(args[1] || 0), Number(args[2] || 0));
        break;
      case "Sprite_Holder.ShowSprite":
        this.setLayerVisible("Sprite_Holder", Number(args[0]), true);
        break;
      case "Sprite_Holder.HideSprite":
        this.setLayerVisible("Sprite_Holder", Number(args[0]), false);
        break;
      case "Sprite_Holder.ChangePhase":
      case "Sprite_Holder.SetDeep":
      case "Sprite_Holder.EnableTimers":
      case "Sprite_Holder.DisableTimers":
      case "Sprite_Holder.SynchronizeTimers":
      case "Sprite_Holder.StopAnimation":
      case "Sprite_Holder.ContinueAnimation":
        this.log(`${local}(${args.join(",")})`);
        break;
      case "HotSpot_Holder.EnableHotSpot":
        this.enableHotspot(Number(args[0]), true);
        break;
      case "HotSpot_Holder.DisableHotSpot":
        this.enableHotspot(Number(args[0]), false);
        break;
      case "HotSpot_Holder.LeftButtonClickOn":
      case "HotSpot_Holder.LeftButtonClickOnUp":
        this.dispatchComponentEvent(
          "HotSpot_Holder",
          local.endsWith("Up") ? "MouseClickOnUp" : "MouseClickOnDown",
          Number(args[0])
        );
        break;
      case "HotSpot_Holder.MoveTo":
        this.log(`${local}(${args.join(",")})`);
        break;
      case "Text_Holder.ShowText":
      case "Text_Holder.HideText":
      case "Text_Holder.PlaySynchroText":
      case "Text_Holder.StopSynchroText":
      case "Text_Holder.SetTextOffsets":
      case "Text_Holder.EnableMouse":
      case "Text_Holder.DisableMouse":
      case "Text_Holder.SetCurrentSelect":
        this.log(`${local}(${args.join(",")})`);
        break;
      default:
        if (!method.includes(".") && this.extractFunctionBody(this.scene?.script?.rawText || "", method)) {
          this.executeHandler(method, args);
        } else if (method.includes(".")) {
          this.log(`stub ${method}(${args.join(",")})`);
        }
        break;
    }
  }
}
