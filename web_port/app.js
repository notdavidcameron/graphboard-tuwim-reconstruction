const state = {
  index: null,
  groups: null,
  scene: null,
  selected: 0,
};

const CACHE_BUST = new URL(document.currentScript?.src || window.location.href).searchParams.get("v") || String(Date.now());

function cacheBustUrl(url) {
  if (!url) return url;
  const parsed = new URL(url, window.location.href);
  parsed.searchParams.set("v", CACHE_BUST);
  return parsed.href;
}

window.GraphBoardCacheBustUrl = cacheBustUrl;

function holderZIndex(namespace, logicalZ, componentOrder = 0, itemOrder = 0) {
  const layer = Number(logicalZ) || 0;
  const component = Math.max(0, Math.min(999, Number(componentOrder) || 0));
  const item = Math.max(0, Math.min(99, Number(itemOrder) || 0));
  // The board walks logical layers top-down. On a tie page components precede
  // group components, earlier components win, and later holder records win.
  const namespaceTie = namespace === "Group" ? 20000 : 60000;
  return 10000000 + layer * 100000 + namespaceTie + (999 - component) * 100 + item;
}

window.GraphBoardZIndex = holderZIndex;

const els = {
  summary: document.getElementById("summary"),
  select: document.getElementById("sceneSelect"),
  prev: document.getElementById("prevBtn"),
  next: document.getElementById("nextBtn"),
  debug: document.getElementById("debugToggle"),
  unknown: document.getElementById("unknownToggle"),
  background: document.getElementById("backgroundToggle"),
  openPage: document.getElementById("openPageBtn"),
  resetRuntime: document.getElementById("resetRuntimeBtn"),
  runtimeLog: document.getElementById("runtimeLog"),
  stage: document.getElementById("stage"),
  componentList: document.getElementById("componentList"),
  audioList: document.getElementById("audioList"),
  scriptInfo: document.getElementById("scriptInfo"),
  handlerSelect: document.getElementById("handlerSelect"),
  handlerArgs: document.getElementById("handlerArgs"),
  runHandler: document.getElementById("runHandlerBtn"),
  warnings: document.getElementById("warnings"),
};

const runtime = new GraphBoardRuntime(els.stage, els.runtimeLog);
runtime.onLoadPage = (bdfName) => {
  const index = state.index.scenes.findIndex((scene) => sceneMatchesQuery(scene, bdfName));
  if (index >= 0) {
    loadScene(index);
  }
};

function esc(value) {
  return String(value ?? "").replace(/[&<>"']/g, (ch) => ({
    "&": "&amp;",
    "<": "&lt;",
    ">": "&gt;",
    '"': "&quot;",
    "'": "&#39;",
  })[ch]);
}

async function getJson(url) {
  const response = await fetch(cacheBustUrl(url), { cache: "no-store" });
  if (!response.ok) {
    throw new Error(`${url}: ${response.status}`);
  }
  return response.json();
}

function sceneEntry() {
  return state.index.scenes[state.selected];
}

function sceneKey(value) {
  return String(value || "").replace(/^.*[\\/]/, "").replace(/\.bdf$/i, "").toLowerCase();
}

function sceneMatchesQuery(scene, query) {
  const wanted = sceneKey(query);
  return sceneKey(scene.id) === wanted || sceneKey(scene.sourceBdf) === wanted;
}

function syncSceneUrl(entry) {
  if (!entry?.id || !window.history?.replaceState) return;
  const url = new URL(window.location.href);
  if (url.searchParams.get("scene") === entry.id) return;
  url.searchParams.set("scene", entry.id);
  window.history.replaceState({ scene: entry.id }, "", url);
}

async function loadScene(selected, options = {}) {
  const { updateUrl = true } = options;
  state.selected = Math.max(0, Math.min(selected, state.index.scenes.length - 1));
  els.select.value = String(state.selected);
  const entry = sceneEntry();
  if (updateUrl) {
    syncSceneUrl(entry);
  }
  state.scene = await getJson(entry.url);
  render();
}

function setStageSize(scene) {
  const rect = scene.pageRect || [0, 0, 640, 480];
  const width = Math.max(1, rect[2] - rect[0]);
  const height = Math.max(1, rect[3] - rect[1]);
  els.stage.style.width = `${width}px`;
  els.stage.style.height = `${height}px`;
}

function runtimeIdFor(component, asset = null) {
  // Wrapper/component-list indexes are not holder item IDs. Asset-less
  // holders expose their first runtime item as 0 in page scripts.
  if (!asset) return 0;
  const id = Number(asset.id ?? 0);
  if (component.type === "Transparent_Video_Holder") {
    return id > 0 && id % 2 === 0 ? id / 2 : id;
  }
  return id;
}

function initialAssetVisible(component, asset, isPrimary, hasUnknownGeometry) {
  if (asset.initiallyVisible === false) return false;
  if (
    component.type === "Sprite_Holder"
    && asset.kind === "sprite_holder_png"
    && Number(asset.width || 0) > 640
    && asset.geometryConfidence !== "serialized_instance"
  ) return false;
  if (!component.visible) return false;
  if (component.type === "Transparent_Video_Holder" && asset.geometryConfidence === "serialized_entry") {
    const runtimeId = runtimeIdFor(component, asset);
    const z = Number(asset.z ?? 0);
    return runtimeId === 0 || z > 0;
  }
  const assetHasGeometry = asset.geometryConfidence && asset.geometryConfidence !== "unknown";
  return isPrimary || els.unknown.checked || !hasUnknownGeometry || assetHasGeometry;
}

function bindRuntimeEvents(element, component, runtimeId, namespace = component.namespace || "Page") {
  element.dataset.runtimeLayer = "1";
  element.dataset.componentType = component.type;
  element.dataset.runtimeId = String(runtimeId);
  element.dataset.runtimeNamespace = namespace;
  element.addEventListener("mouseenter", () => runtime.dispatchComponentEvent(component.type, "MouseMoveIn", runtimeId, [], namespace));
  element.addEventListener("mouseleave", () => runtime.dispatchComponentEvent(component.type, "MouseMoveOut", runtimeId, [], namespace));
  element.addEventListener("mousedown", (event) => {
    if (event.button !== 0) return;
    runtime.runUserInput(() => {
      if (component.type === "Puzzle") runtime.executeHandler("Puzzle.MouseStartDrag");
      if (component.type === "HotSpot_Holder") runtime.dispatchComponentEvent(component.type, "LeftButtonClickOn", runtimeId, [], namespace);
      else if (["Sprite_Holder", "Transparent_Video_Holder", "MultiBitmap", "Bitmap_Holder"].includes(component.type)) {
        const point = stagePointFromEvent(event);
        const extra = component.type === "MultiBitmap" ? [point.x, point.y, Number(element.dataset.logicalZ) || 0] : [];
        runtime.dispatchComponentEvent(component.type, "MouseClickOnDown", runtimeId, extra, namespace);
        if (["Sprite_Holder", "Bitmap_Holder"].includes(component.type)) {
          runtime.beginPointerInteraction(element, component.type, runtimeId, namespace, point);
        }
      }
    });
  });
  element.addEventListener("mouseup", (event) => {
    if (event.button !== 0) return;
    runtime.runUserInput(() => {
      if (component.type === "Puzzle") {
        runtime.puzzle.drops += 1;
        runtime.executeHandler("Puzzle.MouseDrop");
        if (runtime.puzzle.drops >= 3) runtime.executeHandler("Puzzle.GameOver", [runtimeId]);
      }
      if (component.type === "HotSpot_Holder") runtime.dispatchComponentEvent(component.type, "LeftButtonClickOnUp", runtimeId, [], namespace);
      else if (["Transparent_Video_Holder", "MultiBitmap"].includes(component.type)) {
        runtime.dispatchComponentEvent(component.type, "MouseClickOnUp", runtimeId, [], namespace);
      }
    });
  });
  element.addEventListener("click", (event) => {
    event.stopPropagation();
    runtime.runUserInput(() => {
      if (component.type === "Text_Holder" || component.type === "Video_Holder") {
        runtime.dispatchComponentEvent(component.type, component.type === "Text_Holder" ? "ClickOnText" : "MouseClickOn", runtimeId, [], namespace);
      }
    });
  });
  element.addEventListener("dblclick", () => {
    if (component.type === "HotSpot_Holder") runtime.runUserInput(() => runtime.dispatchComponentEvent(component.type, "DBLClickOn", runtimeId, [], namespace));
  });
  element.addEventListener("contextmenu", (event) => {
    event.preventDefault();
    if (component.type === "HotSpot_Holder") runtime.runUserInput(() => runtime.dispatchComponentEvent(component.type, "RightButtonClickOn", runtimeId, [], namespace));
  });
}

function stagePointFromEvent(event) {
  const rect = els.stage.getBoundingClientRect();
  return {
    x: Math.round(event.clientX - rect.left),
    y: Math.round(event.clientY - rect.top),
  };
}

function renderStage(scene) {
  els.stage.replaceChildren();
  setStageSize(scene);

  if (els.background.checked && scene.background?.bmpUrl) {
    const img = document.createElement("img");
    img.className = "stage-bg";
    img.src = cacheBustUrl(scene.background.bmpUrl);
    img.alt = "";
    els.stage.appendChild(img);
  } else {
    const colorIndex = scene.background?.colorIndex ?? 0;
    els.stage.style.background = `hsl(${(colorIndex * 31) % 360} 28% 18%)`;
  }

  const components = [...(scene.components || [])].sort((a, b) => (a.z ?? 0) - (b.z ?? 0));
  for (const component of components) {
    const assets = component.assets || [];
    const hasUnknownGeometry = component.geometryConfidence === "unknown";
    for (const [assetIndex, asset] of assets.entries()) {
      if (!asset.url) continue;
      const rect = asset.rect || component.rect || { x: 0, y: 0, width: 1, height: 1 };
      const layerZ = Number.isFinite(Number(asset.z)) ? Number(asset.z) : Number(component.z ?? 0);
      const assetOrder = Number.isFinite(Number(asset.id)) ? Number(asset.id) : 0;
      const assetHasGeometry = asset.geometryConfidence && asset.geometryConfidence !== "unknown";
      const initialPhaseAsset = component.type === "Sprite_Holder"
        ? asset.phaseAssets?.find((item) => Number(item.phase) === Number(asset.initialPhase || 0))
        : null;
      const initialSrc = component.type === "Transparent_Video_Holder" && asset.stillUrl
        ? asset.stillUrl
        : initialPhaseAsset?.url || asset.url;
      const img = document.createElement("img");
      img.className = "layer";
      img.src = cacheBustUrl(initialSrc);
      img.alt = `${component.id}:${asset.id ?? ""}`;
      img.dataset.originalSrc = cacheBustUrl(asset.url);
      if (asset.stillUrl) img.dataset.stillSrc = cacheBustUrl(asset.stillUrl);
      img.dataset.runtimeLayer = "1";
      img.dataset.runtimeNamespace = "Page";
      img.dataset.componentType = component.type;
      img.dataset.assetId = String(asset.id ?? "");
      img.dataset.runtimeId = String(runtimeIdFor(component, asset));
      if (component.type === "Sprite_Holder") img.dataset.phase = String(asset.initialPhase || 0);
      if (component.type === "Sprite_Holder") img.dataset.draggable = asset.draggable ? "1" : "0";
      img.dataset.runtimeKey = `Page:${component.type}:${asset.id ?? ""}`;
      img.dataset.runtimeLookupKey = `Page:${component.type}:${runtimeIdFor(component, asset)}`;
      img.dataset.componentOrder = String(component.index ?? 0);
      img.dataset.itemOrder = String(assetIndex);
      if (
        component.type === "Sprite_Holder"
        && asset.kind === "sprite_holder_png"
        && Number(asset.width || 0) > 640
        && asset.geometryConfidence !== "serialized_instance"
      ) {
        img.dataset.suppressVisual = "1";
      }
      img.style.left = `${rect.x}px`;
      img.style.top = `${rect.y}px`;
      img.style.width = `${asset.width || rect.width}px`;
      img.style.height = `${asset.height || rect.height}px`;
      img.style.zIndex = String(holderZIndex("Page", layerZ, component.index, assetIndex));
      img.dataset.logicalZ = String(layerZ);
      img.dataset.baseZIndex = img.style.zIndex;
      const isPrimary = component.primaryAssetUrl === asset.url;
      const shouldShow = initialAssetVisible(component, asset, isPrimary, hasUnknownGeometry);
      img.classList.toggle("runtime-hidden", !shouldShow);
      bindRuntimeEvents(img, component, runtimeIdFor(component, asset), "Page");
      els.stage.appendChild(img);

      if (els.debug.checked && assetHasGeometry) {
        const box = document.createElement("div");
        box.className = "bounds asset";
        box.style.left = `${rect.x}px`;
        box.style.top = `${rect.y}px`;
        box.style.width = `${Math.max(rect.width, 18)}px`;
        box.style.height = `${Math.max(rect.height, 14)}px`;
        box.style.zIndex = String(2147480000 + assetOrder);
        box.textContent = `${component.index}:${asset.id ?? ""}`;
        els.stage.appendChild(box);
      }
    }

    const hitboxes = component.hitboxes || [];
    for (const [hotspotIndex, hotspot] of hitboxes.entries()) {
      const rect = hotspot.rect || { x: 0, y: 0, width: 1, height: 1 };
      const hitbox = document.createElement("button");
      hitbox.type = "button";
      hitbox.className = "hitbox";
      hitbox.dataset.runtimeNamespace = "Page";
      hitbox.dataset.componentOrder = String(component.index ?? 0);
      hitbox.dataset.itemOrder = String(hotspotIndex);
      hitbox.dataset.logicalZ = String(Number(hotspot.layer ?? component.z ?? 0));
      hitbox.style.left = `${rect.x}px`;
      hitbox.style.top = `${rect.y}px`;
      hitbox.style.width = `${Math.max(rect.width, 8)}px`;
      hitbox.style.height = `${Math.max(rect.height, 8)}px`;
      hitbox.style.zIndex = String(holderZIndex("Page", hotspot.layer ?? component.z, component.index, hotspotIndex));
      hitbox.dataset.baseZIndex = hitbox.style.zIndex;
      hitbox.title = `${component.type} ${hotspot.id}`;
      bindRuntimeEvents(hitbox, component, Number(hotspot.id || 0), "Page");
      els.stage.appendChild(hitbox);
    }

    const isVisualComponent = component.geometryConfidence !== "not_visual";
    if (
      isVisualComponent
      && component.assets.length === 0
      && hitboxes.length === 0
      && (els.unknown.checked || !hasUnknownGeometry || component.type === "HotSpot_Holder")
    ) {
      const rect = component.rect || { x: 0, y: 0, width: 1, height: 1 };
      const hitbox = document.createElement("button");
      hitbox.type = "button";
      hitbox.className = "hitbox";
      hitbox.dataset.runtimeNamespace = "Page";
      hitbox.dataset.componentOrder = String(component.index ?? 0);
      hitbox.dataset.itemOrder = "0";
      hitbox.dataset.logicalZ = String(Number(component.z ?? 0));
      hitbox.style.left = `${rect.x}px`;
      hitbox.style.top = `${rect.y}px`;
      hitbox.style.width = `${Math.max(rect.width, 8)}px`;
      hitbox.style.height = `${Math.max(rect.height, 8)}px`;
      hitbox.style.zIndex = String(holderZIndex("Page", component.z, component.index, 0));
      hitbox.dataset.baseZIndex = hitbox.style.zIndex;
      const runtimeId = runtimeIdFor(component);
      hitbox.title = `${component.type} ${runtimeId}`;
      bindRuntimeEvents(hitbox, component, runtimeId, "Page");
      els.stage.appendChild(hitbox);
    }

    if (els.debug.checked && isVisualComponent && (els.unknown.checked || !hasUnknownGeometry || component.visible)) {
      const rect = component.rect || { x: 0, y: 0, width: 1, height: 1 };
      const box = document.createElement("div");
      box.className = `bounds ${component.geometryConfidence === "unknown" ? "unknown" : ""}`;
      box.style.left = `${rect.x}px`;
      box.style.top = `${rect.y}px`;
      box.style.width = `${Math.max(rect.width, 24)}px`;
      box.style.height = `${Math.max(rect.height, 18)}px`;
      box.style.zIndex = String(2147470000 + (component.index ?? 0));
      box.textContent = `${component.index}: ${component.type} (${component.geometryConfidence})`;
      els.stage.appendChild(box);
    }
  }
}

function renderComponents(scene) {
  const rows = (scene.components || []).map((component) => {
    const assets = component.assets?.length || 0;
    const audio = component.audio?.length || 0;
    const rect = component.rect || {};
    return `
      <div class="item">
        <strong>${esc(component.index)}. ${esc(component.type)}</strong>
        <small>${esc(component.geometryConfidence)} rect ${esc(rect.x)},${esc(rect.y)} ${esc(rect.width)}x${esc(rect.height)}</small>
      <small>${assets} image asset(s), ${audio} audio asset(s), ${component.hitboxes?.length || 0} hitbox(es), offset ${esc(component.sourceOffset)}</small>
      </div>
    `;
  });
  els.componentList.innerHTML = rows.join("") || `<div class="item"><small>No components.</small></div>`;
}

function renderAudio(scene) {
  const audioAssets = [];
  for (const component of scene.components || []) {
    for (const asset of component.audio || []) {
      audioAssets.push({ component, asset });
    }
  }
  if (audioAssets.length === 0) {
    els.audioList.innerHTML = `<div class="item compact-empty"><small>No audio assets.</small></div>`;
    return;
  }
  const rows = audioAssets.slice(0, 120).map(({ component, asset }, index) => `
    <div class="audio-row">
      <span class="audio-label">${index + 1}. ${esc(component.type)} #${esc(runtimeIdFor(component, asset))}</span>
      <span class="audio-kind">${esc(asset.kind)}${asset.durationSeconds ? ` ${esc(asset.durationSeconds)}s` : ""}</span>
      <audio controls preload="none" src="${esc(cacheBustUrl(asset.url))}" data-runtime-namespace="Page" data-component-type="${esc(component.type)}" data-asset-id="${esc(asset.id)}" data-runtime-id="${esc(runtimeIdFor(component, asset))}" data-runtime-key="Page:${esc(component.type)}:${esc(asset.id)}" data-duration-seconds="${esc(asset.durationSeconds || "")}"></audio>
    </div>
  `).join("");
  els.audioList.innerHTML = `
    <details class="audio-pack">
      <summary>${audioAssets.length} audio asset(s)</summary>
      <div class="audio-grid">${rows}</div>
    </details>
  `;
}

function renderScript(scene) {
  const script = scene.script || {};
  const handlers = script.handlers || [];
  els.handlerSelect.innerHTML = handlers.map((handler) => (
    `<option value="${esc(handler.name)}">${esc(handler.signature)}</option>`
  )).join("");
  els.handlerSelect.disabled = handlers.length === 0;
  els.handlerArgs.disabled = handlers.length === 0;
  els.runHandler.disabled = handlers.length === 0;
  els.scriptInfo.innerHTML = `
    <div>${script.url ? `<a href="${esc(script.url)}" target="_blank" rel="noreferrer">script.txt</a>` : "No script file"}</div>
    <div>${handlers.length} handler(s)</div>
    ${handlers.slice(0, 30).map((handler) => `<div>${esc(handler.signature)}</div>`).join("")}
  `;
}

function renderWarnings(scene) {
  const warnings = scene.warnings || [];
  els.warnings.innerHTML = warnings.map((warning) => `<div>${esc(warning)}</div>`).join("");
}

function render() {
  const scene = state.scene;
  const entry = sceneEntry();
  document.title = `${entry.title} - GraphBoard Scene Viewer`;
  renderStage(scene);
  renderComponents(scene);
  renderAudio(scene);
  renderScript(scene);
  renderWarnings(scene);
  runtime.reset(scene);
  window.requestAnimationFrame(() => runtime.executeHandler("OnOpenPage"));
}

async function boot() {
  state.index = await getJson("scenes/index.json");
  state.groups = await getJson(state.index.groupsUrl || "groups/index.json").catch(() => ({ groups: [] }));
  const loadedGroups = await Promise.all((state.groups.groups || []).map((group) => getJson(group.url)));
  runtime.setGroups(loadedGroups);
  els.summary.textContent = `${state.index.scenes.length} pages, ${loadedGroups.length} groups, ${state.index.validation.totalComponents} components, ${state.index.validation.totalAssets} assets`;
  els.select.innerHTML = state.index.scenes.map((scene, index) => (
    `<option value="${index}">${esc(scene.title)} (${scene.componentCount})</option>`
  )).join("");

  els.select.addEventListener("change", () => loadScene(Number(els.select.value)));
  els.prev.addEventListener("click", () => loadScene(state.selected - 1));
  els.next.addEventListener("click", () => loadScene(state.selected + 1));
  els.openPage.addEventListener("click", () => runtime.executeHandler("OnOpenPage"));
  els.resetRuntime.addEventListener("click", render);
  els.runHandler.addEventListener("click", () => {
    const args = els.handlerArgs.value
      .split(",")
      .map((item) => item.trim())
      .filter(Boolean)
      .map((item) => (/^-?\d+$/.test(item) ? Number(item) : item));
    runtime.executeHandler(els.handlerSelect.value, args);
  });
  els.stage.addEventListener("mousedown", (event) => {
    if (event.button !== 0) return;
    const point = stagePointFromEvent(event);
    runtime.executeHandlerIfPresent("OnLButtonDown", [point.x, point.y]);
  }, true);
  els.stage.addEventListener("mouseup", (event) => {
    if (event.button !== 0) return;
    const point = stagePointFromEvent(event);
    runtime.executeHandlerIfPresent("OnLButtonUp", [point.x, point.y]);
    runtime.endPointerInteraction(point);
  }, true);
  els.stage.addEventListener("contextmenu", (event) => {
    event.preventDefault();
    const point = stagePointFromEvent(event);
    runtime.executeHandlerIfPresent("OnRButtonDown", [point.x, point.y]);
  }, true);
  els.stage.addEventListener("mousemove", (event) => {
    const point = stagePointFromEvent(event);
    runtime.updatePointerInteraction(point);
    runtime.handleStagePointer(point);
  });
  els.stage.addEventListener("mouseleave", () => runtime.handleStagePointer(null));
  for (const input of [els.debug, els.unknown, els.background]) {
    input.addEventListener("change", render);
  }

  const params = new URLSearchParams(location.search);
  const wanted = params.get("scene");
  const selected = wanted
    ? state.index.scenes.findIndex((scene) => sceneMatchesQuery(scene, wanted))
    : 0;
  await loadScene(selected >= 0 ? selected : 0);
}

boot().catch((error) => {
  els.summary.textContent = "Failed to load scene index";
  els.warnings.textContent = error.stack || String(error);
});
