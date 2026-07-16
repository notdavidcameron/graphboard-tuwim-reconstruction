const state = {
  index: null,
  groupIndex: null,
  groups: [],
  media: { assets: [] },
  document: null,
  mode: "scene",
  selected: { scene: 0, group: 0, loose: 0 },
  loadToken: 0,
  summary: "",
  records: [],
  selectedRecord: null,
  componentFilter: null,
  zoom: { mode: "fit", value: 1 },
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
  const namespaceTie = namespace === "Group" ? 20000 : 60000;
  return 10000000 + layer * 100000 + namespaceTie + (999 - component) * 100 + item;
}

function holderInputZIndex(componentOrder = 0, itemOrder = 0) {
  const component = Math.max(0, Math.min(999, Number(componentOrder) || 0));
  const item = Math.max(0, Math.min(99, Number(itemOrder) || 0));
  return 2140000000 + (999 - component) * 100 + item;
}

function holderTextZIndex(componentOrder = 0, itemOrder = 0) {
  const component = Math.max(0, Math.min(999, Number(componentOrder) || 0));
  const item = Math.max(0, Math.min(99, Number(itemOrder) || 0));
  return 2130000000 + (999 - component) * 100 + item;
}

function holderDragZIndex(componentOrder = 0, itemOrder = 0) {
  const component = Math.max(0, Math.min(999, Number(componentOrder) || 0));
  const item = Math.max(0, Math.min(99, Number(itemOrder) || 0));
  return 2141000000 + (999 - component) * 100 + item;
}

window.GraphBoardZIndex = holderZIndex;
window.GraphBoardInputZIndex = holderInputZIndex;
window.GraphBoardDragZIndex = holderDragZIndex;

const els = {
  summary: document.getElementById("summary"),
  select: document.getElementById("sceneSelect"),
  prev: document.getElementById("prevBtn"),
  next: document.getElementById("nextBtn"),
  search: document.getElementById("archiveSearch"),
  collectionList: document.getElementById("collectionList"),
  collectionLabel: document.getElementById("collectionLabel"),
  filteredCount: document.getElementById("filteredCount"),
  sceneCount: document.getElementById("sceneCount"),
  groupCount: document.getElementById("groupCount"),
  mediaCount: document.getElementById("mediaCount"),
  debug: document.getElementById("debugToggle"),
  unknown: document.getElementById("unknownToggle"),
  background: document.getElementById("backgroundToggle"),
  openPage: document.getElementById("openPageBtn"),
  resetRuntime: document.getElementById("resetRuntimeBtn"),
  runtimeLog: document.getElementById("runtimeLog"),
  stage: document.getElementById("stage"),
  stageWrap: document.getElementById("stageWrap"),
  stageScaler: document.getElementById("stageScaler"),
  canvasShell: document.getElementById("canvasShell"),
  mediaBrowser: document.getElementById("mediaBrowser"),
  documentType: document.getElementById("documentType"),
  documentTitle: document.getElementById("documentTitle"),
  documentMeta: document.getElementById("documentMeta"),
  stageDimensions: document.getElementById("stageDimensions"),
  pointerPosition: document.getElementById("pointerPosition"),
  layerCount: document.getElementById("layerCount"),
  fit: document.getElementById("fitBtn"),
  actual: document.getElementById("actualBtn"),
  zoomOut: document.getElementById("zoomOutBtn"),
  zoomIn: document.getElementById("zoomInBtn"),
  zoomValue: document.getElementById("zoomValue"),
  assetGrid: document.getElementById("assetGrid"),
  assetSummary: document.getElementById("assetSummary"),
  assetFilter: document.getElementById("assetFilter"),
  assetSearch: document.getElementById("assetSearch"),
  selectionPreview: document.getElementById("selectionPreview"),
  selectionInfo: document.getElementById("selectionInfo"),
  componentList: document.getElementById("componentList"),
  componentCount: document.getElementById("componentCount"),
  audioList: document.getElementById("audioList"),
  scriptInfo: document.getElementById("scriptInfo"),
  handlerSelect: document.getElementById("handlerSelect"),
  handlerArgs: document.getElementById("handlerArgs"),
  runHandler: document.getElementById("runHandlerBtn"),
  warnings: document.getElementById("warnings"),
  rawJson: document.getElementById("rawJson"),
  copyRaw: document.getElementById("copyRawBtn"),
  openJson: document.getElementById("openJsonLink"),
};

const runtime = new GraphBoardRuntime(els.stage, els.runtimeLog);
runtime.onLoadPage = (bdfName) => {
  const index = state.index.scenes.findIndex((scene) => sceneMatchesQuery(scene, bdfName));
  if (index >= 0) {
    setMode("scene", false);
    loadDocument(index);
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

function formatBytes(value) {
  const size = Number(value);
  if (!Number.isFinite(size)) return "—";
  if (size < 1024) return size + " B";
  if (size < 1048576) return (size / 1024).toFixed(1) + " KB";
  return (size / 1048576).toFixed(1) + " MB";
}

function basename(value) {
  return String(value || "").replace(/^.*[\\/]/, "");
}

async function getJson(url) {
  const response = await fetch(cacheBustUrl(url), { cache: "no-store" });
  if (!response.ok) throw new Error(url + ": " + response.status);
  return response.json();
}

function sceneKey(value) {
  return basename(value).replace(/\.bdf$/i, "").toLowerCase();
}

function sceneMatchesQuery(scene, query) {
  const wanted = sceneKey(query);
  return sceneKey(scene.id) === wanted || sceneKey(scene.sourceBdf) === wanted;
}

function entriesForMode(mode = state.mode) {
  if (mode === "group") return state.groupIndex?.groups || [];
  if (mode === "loose") return state.media?.assets || [];
  return state.index?.scenes || [];
}

function currentEntry() {
  return entriesForMode()[state.selected[state.mode]] || null;
}

function entrySubtitle(entry) {
  if (state.mode === "scene") return entry.componentCount + " components · " + entry.assetCount + " assets";
  if (state.mode === "group") return entry.componentCount + " components · " + entry.cursorCount + " cursors";
  return (entry.mediaType || entry.extension || "file").toUpperCase() + " · " + formatBytes(entry.size);
}

function entryBadge(entry) {
  if (state.mode === "scene") return String(entry.assetCount || 0);
  if (state.mode === "group") return String(entry.assetCount || 0);
  return entry.extension || "";
}

function renderLibrary() {
  const entries = entriesForMode();
  const query = els.search.value.trim().toLowerCase();
  const filtered = entries.map((entry, index) => ({ entry, index })).filter(({ entry }) => {
    return !query || [entry.id, entry.title, entry.name, entry.sourceBdf, entry.sourceGrp, entry.path, entry.mediaType]
      .some((value) => String(value || "").toLowerCase().includes(query));
  });
  els.filteredCount.textContent = filtered.length === entries.length ? String(entries.length) : filtered.length + " / " + entries.length;
  els.collectionLabel.textContent = state.mode === "scene" ? "BDF scenes" : state.mode === "group" ? "GRP libraries" : "Loose media";
  els.collectionList.innerHTML = filtered.map(({ entry, index }) => (
    '<button type="button" class="collection-item ' + (index === state.selected[state.mode] ? "active" : "") + '" data-entry-index="' + index + '" role="option" aria-selected="' + (index === state.selected[state.mode]) + '">' +
      '<span><span class="collection-name">' + esc(entry.title || entry.name || entry.id) + '</span><span class="collection-kind">' + esc(entrySubtitle(entry)) + '</span></span>' +
      '<span class="collection-badge">' + esc(entryBadge(entry)) + '</span>' +
    '</button>'
  )).join("") || '<div class="empty-list">No matching archive items.</div>';
  els.select.innerHTML = entries.map((entry, index) => (
    '<option value="' + index + '">' + esc(entry.title || entry.name || entry.id) + '</option>'
  )).join("");
  els.select.value = String(state.selected[state.mode]);
  els.prev.disabled = state.selected[state.mode] <= 0;
  els.next.disabled = state.selected[state.mode] >= entries.length - 1;
}

function setMode(mode, shouldLoad = true) {
  if (!["scene", "group", "loose"].includes(mode)) return;
  state.mode = mode;
  state.componentFilter = null;
  els.search.value = "";
  els.assetSearch.value = "";
  document.querySelectorAll(".mode-tab").forEach((button) => button.classList.toggle("active", button.dataset.mode === mode));
  els.search.placeholder = mode === "scene" ? "Search BDF pages…" : mode === "group" ? "Search GRP libraries…" : "Search WAV / AVI media…";
  renderLibrary();
  if (shouldLoad) loadDocument(state.selected[mode] || 0);
}

function syncUrl(entry) {
  if (!entry || !window.history?.replaceState) return;
  const url = new URL(window.location.href);
  url.searchParams.delete("scene");
  url.searchParams.delete("group");
  url.searchParams.delete("media");
  const key = state.mode === "scene" ? "scene" : state.mode === "group" ? "group" : "media";
  url.searchParams.set(key, entry.id);
  window.history.replaceState({ mode: state.mode, id: entry.id }, "", url);
}

function setNavigationEnabled(enabled) {
  [els.select, els.prev, els.next].forEach((control) => { control.disabled = !enabled; });
}

async function loadDocument(selected, options = {}) {
  const entries = entriesForMode();
  if (!entries.length) return false;
  const next = Math.max(0, Math.min(Number(selected) || 0, entries.length - 1));
  const requestToken = ++state.loadToken;
  state.selected[state.mode] = next;
  state.componentFilter = null;
  state.selectedRecord = null;
  const entry = entries[next];
  renderLibrary();
  if (options.updateUrl !== false) syncUrl(entry);
  setNavigationEnabled(false);
  els.summary.textContent = "Loading " + (entry.title || entry.name || entry.id) + "…";
  try {
    if (state.mode === "loose") {
      state.document = {
        id: entry.id,
        title: entry.title || entry.name,
        sourceMedia: entry.path,
        pageRect: [0, 0, 640, 480],
        components: [],
        warnings: [],
        media: entry,
      };
    } else {
      state.document = await getJson(entry.url);
    }
    if (requestToken !== state.loadToken) return false;
    render();
    els.summary.textContent = state.summary;
    return true;
  } catch (error) {
    if (requestToken !== state.loadToken) return false;
    els.summary.textContent = "Failed to load " + (entry.title || entry.id);
    renderWarnings({ warnings: [error?.message || String(error)] });
    return false;
  } finally {
    if (requestToken === state.loadToken) setNavigationEnabled(true);
  }
}

function pageSize(scene = state.document) {
  const rect = scene?.pageRect || [0, 0, 640, 480];
  return {
    width: Math.max(1, Number(rect[2]) - Number(rect[0])),
    height: Math.max(1, Number(rect[3]) - Number(rect[1])),
  };
}

function setStageSize(scene) {
  const size = pageSize(scene);
  els.stage.style.width = size.width + "px";
  els.stage.style.height = size.height + "px";
  els.stageDimensions.textContent = size.width + " × " + size.height;
  applyZoom();
}

function applyZoom() {
  if (state.mode === "loose") return;
  const size = pageSize();
  if (state.zoom.mode === "fit") {
    const availableWidth = Math.max(120, els.stageWrap.clientWidth - 48);
    const availableHeight = Math.max(100, els.stageWrap.clientHeight - 48);
    state.zoom.value = Math.min(1.5, availableWidth / size.width, availableHeight / size.height);
  }
  const zoom = Math.max(.2, Math.min(3, state.zoom.value));
  state.zoom.value = zoom;
  els.stage.style.transform = "scale(" + zoom + ")";
  els.stageScaler.style.width = Math.round(size.width * zoom) + "px";
  els.stageScaler.style.height = Math.round(size.height * zoom) + "px";
  els.zoomValue.textContent = Math.round(zoom * 100) + "%";
  els.fit.classList.toggle("active", state.zoom.mode === "fit");
  els.actual.classList.toggle("active", state.zoom.mode === "actual");
}

function setZoom(mode, value) {
  state.zoom.mode = mode;
  if (Number.isFinite(value)) state.zoom.value = value;
  applyZoom();
}

function runtimeIdFor(component, asset = null) {
  if (!asset) return 0;
  const id = Number(asset.id ?? 0);
  if (component.type === "Transparent_Video_Holder") return id > 0 && id % 2 === 0 ? id / 2 : id;
  return id;
}

function initialAssetVisible(component, asset, isPrimary, hasUnknownGeometry) {
  if (asset.initiallyVisible === false) return false;
  if (component.type === "Sprite_Holder" && asset.kind === "sprite_holder_png" && Number(asset.width || 0) > 640 && asset.geometryConfidence !== "serialized_instance") return false;
  if (!component.visible) return false;
  if (component.type === "Transparent_Video_Holder" && asset.geometryConfidence === "serialized_entry") {
    return runtimeIdFor(component, asset) === 0 || Number(asset.z ?? 0) > 0;
  }
  const assetHasGeometry = asset.geometryConfidence && asset.geometryConfidence !== "unknown";
  return isPrimary || els.unknown.checked || !hasUnknownGeometry || assetHasGeometry;
}

function bindRuntimeEvents(element, component, runtimeId, namespace) {
  element.dataset.runtimeLayer = "1";
  element.dataset.componentType = component.type;
  element.dataset.runtimeId = String(runtimeId);
  element.dataset.runtimeNamespace = namespace;
  element.addEventListener("mouseenter", () => runtime.dispatchComponentEvent(component.type, "MouseMoveIn", runtimeId, [], namespace));
  element.addEventListener("mouseleave", () => runtime.dispatchComponentEvent(component.type, "MouseMoveOut", runtimeId, [], namespace));
  element.addEventListener("mousedown", (event) => {
    if (event.button !== 0 || state.mode !== "scene") return;
    runtime.runUserInput(() => {
      if (component.type === "Puzzle") runtime.executeHandler("Puzzle.MouseStartDrag");
      if (component.type === "HotSpot_Holder") runtime.dispatchComponentEvent(component.type, "LeftButtonClickOn", runtimeId, [], namespace);
      else if (["Sprite_Holder", "Transparent_Video_Holder", "MultiBitmap", "Bitmap_Holder"].includes(component.type)) {
        const point = stagePointFromEvent(event);
        const extra = component.type === "MultiBitmap" ? [point.x, point.y, Number(element.dataset.logicalZ) || 0] : [];
        runtime.dispatchComponentEvent(component.type, "MouseClickOnDown", runtimeId, extra, namespace);
        if (["Sprite_Holder", "Bitmap_Holder"].includes(component.type)) runtime.beginPointerInteraction(element, component.type, runtimeId, namespace, point);
      }
    });
  });
  element.addEventListener("mouseup", (event) => {
    if (event.button !== 0 || state.mode !== "scene") return;
    runtime.runUserInput(() => {
      if (component.type === "Puzzle") {
        runtime.puzzle.drops += 1;
        runtime.executeHandler("Puzzle.MouseDrop");
        if (runtime.puzzle.drops >= 3) runtime.executeHandler("Puzzle.GameOver", [runtimeId]);
      }
      if (component.type === "HotSpot_Holder") runtime.dispatchComponentEvent(component.type, "LeftButtonClickOnUp", runtimeId, [], namespace);
      else if (["Transparent_Video_Holder", "MultiBitmap"].includes(component.type)) runtime.dispatchComponentEvent(component.type, "MouseClickOnUp", runtimeId, [], namespace);
    });
  });
  element.addEventListener("click", (event) => {
    event.stopPropagation();
    if (state.mode !== "scene") return;
    runtime.runUserInput(() => {
      if (component.type === "Text_Holder" || component.type === "Video_Holder") {
        runtime.dispatchComponentEvent(component.type, component.type === "Text_Holder" ? "ClickOnText" : "MouseClickOn", runtimeId, [], namespace);
      }
    });
  });
  element.addEventListener("dblclick", () => {
    if (state.mode === "scene" && component.type === "HotSpot_Holder") runtime.runUserInput(() => runtime.dispatchComponentEvent(component.type, "DBLClickOn", runtimeId, [], namespace));
  });
  element.addEventListener("contextmenu", (event) => {
    if (component.type !== "HotSpot_Holder") return;
    event.preventDefault();
    if (state.mode === "scene") runtime.runUserInput(() => runtime.dispatchComponentEvent(component.type, "RightButtonClickOn", runtimeId, [], namespace));
  });
}

function stagePointFromEvent(event) {
  const rect = els.stage.getBoundingClientRect();
  return {
    x: Math.max(0, Math.round((event.clientX - rect.left) / state.zoom.value)),
    y: Math.max(0, Math.round((event.clientY - rect.top) / state.zoom.value)),
  };
}

function recordKey(component, asset, prefix = "asset") {
  return prefix + ":" + component.id + ":" + String(asset.id ?? asset.index ?? asset.name ?? 0);
}

function renderStage(scene) {
  els.stage.replaceChildren();
  setStageSize(scene);
  const namespace = state.mode === "group" ? "Group" : "Page";
  const showBackground = state.mode === "scene" && els.background.checked && scene.background?.bmpUrl;
  if (showBackground) {
    const img = document.createElement("img");
    img.className = "stage-bg";
    img.src = cacheBustUrl(scene.background.bmpUrl);
    img.alt = "";
    img.dataset.inspectKey = "background";
    img.addEventListener("click", () => selectRecord("background"));
    els.stage.appendChild(img);
  } else {
    const colorIndex = scene.background?.colorIndex ?? 0;
    els.stage.style.background = state.mode === "group" ? "#11141a" : "hsl(" + ((colorIndex * 31) % 360) + " 28% 18%)";
  }

  let visualCount = showBackground ? 1 : 0;
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
      const initialSrc = component.type === "Transparent_Video_Holder" && asset.stillUrl ? asset.stillUrl : initialPhaseAsset?.url || asset.url;
      const img = document.createElement("img");
      const inspectKey = recordKey(component, asset);
      img.className = "layer";
      img.src = cacheBustUrl(initialSrc);
      img.alt = component.id + ":" + (asset.id ?? "");
      img.dataset.inspectKey = inspectKey;
      img.dataset.originalSrc = cacheBustUrl(asset.url);
      if (asset.stillUrl) img.dataset.stillSrc = cacheBustUrl(asset.stillUrl);
      img.dataset.runtimeLayer = "1";
      img.dataset.runtimeNamespace = namespace;
      img.dataset.componentType = component.type;
      img.dataset.assetId = String(asset.id ?? "");
      img.dataset.runtimeId = String(runtimeIdFor(component, asset));
      if (component.type === "Sprite_Holder") img.dataset.phase = String(asset.initialPhase || 0);
      if (component.type === "Sprite_Holder") img.dataset.draggable = asset.draggable ? "1" : "0";
      img.dataset.runtimeKey = namespace + ":" + component.type + ":" + (asset.id ?? "");
      img.dataset.runtimeLookupKey = namespace + ":" + component.type + ":" + runtimeIdFor(component, asset);
      img.dataset.componentOrder = String(component.index ?? 0);
      img.dataset.itemOrder = String(assetIndex);
      if (component.type === "Sprite_Holder" && asset.kind === "sprite_holder_png" && Number(asset.width || 0) > 640 && asset.geometryConfidence !== "serialized_instance") img.dataset.suppressVisual = "1";
      img.style.left = rect.x + "px";
      img.style.top = rect.y + "px";
      img.style.width = (asset.width || rect.width) + "px";
      img.style.height = (asset.height || rect.height) + "px";
      const visualZ = component.type === "Sprite_Holder" && asset.draggable
        ? holderDragZIndex(component.index, assetIndex)
        : holderZIndex(namespace, layerZ, component.index, assetIndex);
      img.style.zIndex = String(visualZ);
      img.dataset.logicalZ = String(layerZ);
      img.dataset.baseZIndex = img.style.zIndex;
      const isPrimary = component.primaryAssetUrl === asset.url;
      img.classList.toggle("runtime-hidden", !initialAssetVisible(component, asset, isPrimary, hasUnknownGeometry));
      bindRuntimeEvents(img, component, runtimeIdFor(component, asset), namespace);
      img.addEventListener("click", () => selectRecord(inspectKey));
      els.stage.appendChild(img);
      visualCount += 1;

      if (els.debug.checked && assetHasGeometry) {
        const box = document.createElement("div");
        box.className = "bounds asset";
        box.style.left = rect.x + "px";
        box.style.top = rect.y + "px";
        box.style.width = Math.max(rect.width, 18) + "px";
        box.style.height = Math.max(rect.height, 14) + "px";
        box.style.zIndex = String(2147480000 + assetOrder);
        box.textContent = component.index + ":" + (asset.id ?? "");
        els.stage.appendChild(box);
      }
    }

    const hitboxes = component.hitboxes || [];
    for (const [hotspotIndex, hotspot] of hitboxes.entries()) {
      const rect = hotspot.rect || { x: 0, y: 0, width: 1, height: 1 };
      const hitbox = document.createElement("button");
      const inspectKey = recordKey(component, hotspot, "hotspot");
      hitbox.type = "button";
      hitbox.className = "hitbox";
      hitbox.dataset.inspectKey = inspectKey;
      hitbox.dataset.runtimeNamespace = namespace;
      hitbox.dataset.componentOrder = String(component.index ?? 0);
      hitbox.dataset.itemOrder = String(hotspotIndex);
      hitbox.dataset.logicalZ = String(Number(hotspot.layer ?? component.z ?? 0));
      hitbox.style.left = rect.x + "px";
      hitbox.style.top = rect.y + "px";
      hitbox.style.width = Math.max(rect.width, 8) + "px";
      hitbox.style.height = Math.max(rect.height, 8) + "px";
      hitbox.style.zIndex = String(holderInputZIndex(component.index, hotspotIndex));
      hitbox.dataset.baseZIndex = hitbox.style.zIndex;
      hitbox.title = component.type + " " + hotspot.id;
      bindRuntimeEvents(hitbox, component, Number(hotspot.id || 0), namespace);
      hitbox.addEventListener("click", () => selectRecord(inspectKey));
      els.stage.appendChild(hitbox);
      visualCount += 1;
    }

    const isVisualComponent = component.geometryConfidence !== "not_visual";
    if (isVisualComponent && assets.length === 0 && hitboxes.length === 0 && (els.unknown.checked || !hasUnknownGeometry || component.type === "HotSpot_Holder")) {
      const rect = component.rect || { x: 0, y: 0, width: 1, height: 1 };
      if (component.type === "Text_Holder") {
        const textLayer = runtime.createSyntheticLayer(namespace, "Text_Holder", runtimeIdFor(component), rect);
        textLayer.dataset.componentOrder = String(component.index ?? 0);
        textLayer.dataset.itemOrder = "0";
        textLayer.style.zIndex = String(holderTextZIndex(component.index, 0));
        textLayer.dataset.baseZIndex = textLayer.style.zIndex;
      } else {
        const hitbox = document.createElement("button");
        hitbox.type = "button";
        hitbox.className = "hitbox";
        hitbox.dataset.runtimeNamespace = namespace;
        hitbox.dataset.componentOrder = String(component.index ?? 0);
        hitbox.dataset.itemOrder = "0";
        hitbox.dataset.logicalZ = String(Number(component.z ?? 0));
        hitbox.style.left = rect.x + "px";
        hitbox.style.top = rect.y + "px";
        hitbox.style.width = Math.max(rect.width, 8) + "px";
        hitbox.style.height = Math.max(rect.height, 8) + "px";
        hitbox.style.zIndex = String(holderInputZIndex(component.index, 0));
        hitbox.dataset.baseZIndex = hitbox.style.zIndex;
        bindRuntimeEvents(hitbox, component, runtimeIdFor(component), namespace);
        els.stage.appendChild(hitbox);
      }
    }

    if (els.debug.checked && isVisualComponent && (els.unknown.checked || !hasUnknownGeometry || component.visible)) {
      const rect = component.rect || { x: 0, y: 0, width: 1, height: 1 };
      const box = document.createElement("div");
      box.className = "bounds " + (component.geometryConfidence === "unknown" ? "unknown" : "");
      box.style.left = rect.x + "px";
      box.style.top = rect.y + "px";
      box.style.width = Math.max(rect.width, 24) + "px";
      box.style.height = Math.max(rect.height, 18) + "px";
      box.style.zIndex = String(2147470000 + (component.index ?? 0));
      box.textContent = component.index + ": " + component.type + " (" + component.geometryConfidence + ")";
      els.stage.appendChild(box);
    }
  }
  els.layerCount.textContent = visualCount + (visualCount === 1 ? " layer" : " layers");
}

function imageRecord(key, title, kind, asset, component = null, extra = {}) {
  return {
    key,
    title: title || asset.name || basename(asset.path) || kind,
    kind,
    type: extra.type || "asset",
    url: asset.url || null,
    previewUrl: extra.previewUrl || asset.stillUrl || asset.url || null,
    asset,
    component,
    ...extra,
  };
}

function flattenRecords(scene) {
  if (state.mode === "loose") {
    return (state.media.assets || []).map((asset) => imageRecord("media:" + asset.id, asset.title || asset.name, asset.mediaType, asset, null, { type: "media" }));
  }
  const records = [];
  if (scene.background?.bmpUrl) {
    records.push(imageRecord("background", "Page background", "background", {
      url: scene.background.bmpUrl,
      path: scene.background.bmpPath,
      width: pageSize(scene).width,
      height: pageSize(scene).height,
    }, null, { type: "background" }));
  }
  for (const cursor of scene.cursors || []) {
    records.push(imageRecord("cursor:" + cursor.index, cursor.name || "Cursor " + cursor.index, "cursor", cursor, null, { type: "cursor" }));
  }
  for (const component of scene.components || []) {
    for (const asset of component.assets || []) {
      const key = recordKey(component, asset);
      records.push(imageRecord(key, asset.name || component.type + " #" + runtimeIdFor(component, asset), asset.kind, asset, component));
      for (const phase of asset.phaseAssets || []) {
        records.push(imageRecord(key + ":phase:" + phase.phase, (asset.name || component.type + " #" + asset.id) + " — phase " + phase.phase, asset.kind + "_phase", {
          ...phase,
          width: asset.width,
          height: asset.height,
          definitionId: asset.definitionId,
        }, component, { type: "phase", parentAsset: asset }));
      }
    }
    for (const asset of component.audio || []) {
      records.push(imageRecord("audio:" + component.id + ":" + (asset.id ?? asset.offset ?? records.length), asset.name || component.type + " audio #" + runtimeIdFor(component, asset), asset.kind || "audio", asset, component, { type: "audio" }));
    }
    for (const hotspot of component.hitboxes || []) {
      records.push(imageRecord(recordKey(component, hotspot, "hotspot"), "Hotspot #" + hotspot.id, "hotspot", hotspot, component, { type: "hotspot" }));
    }
  }
  if (scene.script?.url) {
    records.push(imageRecord("script", "Recovered page script", "script", { url: scene.script.url, path: scene.script.path }, null, { type: "script" }));
  }
  return records;
}

function recordSearchText(record) {
  return [record.title, record.kind, record.type, record.asset?.name, record.asset?.path, record.component?.type, record.component?.id].join(" ").toLowerCase();
}

function renderAssetGrid() {
  state.records = flattenRecords(state.document);
  const kinds = [...new Set(state.records.map((record) => record.kind))].sort();
  const previousKind = els.assetFilter.value || "all";
  els.assetFilter.innerHTML = '<option value="all">All types</option>' + kinds.map((kind) => '<option value="' + esc(kind) + '">' + esc(kind) + '</option>').join("");
  els.assetFilter.value = kinds.includes(previousKind) ? previousKind : "all";
  const query = els.assetSearch.value.trim().toLowerCase();
  const filterKind = els.assetFilter.value;
  const filtered = state.records.filter((record) => {
    if (filterKind !== "all" && record.kind !== filterKind) return false;
    if (state.componentFilter && record.component?.id !== state.componentFilter) return false;
    return !query || recordSearchText(record).includes(query);
  });
  els.assetSummary.textContent = filtered.length + " shown · " + state.records.length + " recovered";
  els.assetGrid.innerHTML = filtered.map((record) => {
    const mediaIcon = record.type === "audio" || record.kind === "audio" ? "♪" : record.kind === "video" ? "▶" : record.type === "hotspot" ? "◎" : record.type === "script" ? "{ }" : "◫";
    const preview = record.previewUrl && !["audio", "video", "script"].includes(record.type) && !["audio", "video", "script"].includes(record.kind)
      ? '<img loading="lazy" src="' + esc(cacheBustUrl(record.previewUrl)) + '" alt="">'
      : '<span class="asset-icon">' + mediaIcon + '</span>';
    const dims = record.asset?.width && record.asset?.height ? record.asset.width + "×" + record.asset.height : record.asset?.size ? formatBytes(record.asset.size) : record.kind;
    return '<button type="button" class="asset-card ' + (state.selectedRecord?.key === record.key ? "selected" : "") + '" data-record-key="' + esc(record.key) + '">' +
      '<span class="asset-thumb">' + preview + '</span>' +
      '<span class="asset-card-body"><span class="asset-card-title">' + esc(record.title) + '</span><span class="asset-card-meta">' + esc(dims) + ' · ' + esc(record.kind) + '</span></span>' +
    '</button>';
  }).join("") || '<div class="empty-list">No assets match this filter.</div>';

  if (!state.selectedRecord || !state.records.some((record) => record.key === state.selectedRecord.key)) {
    const preferredKey = state.mode === "loose" ? "media:" + state.document.media.id : (state.records[0]?.key || null);
    state.selectedRecord = state.records.find((record) => record.key === preferredKey) || state.records[0] || null;
  }
  renderSelection();
}

function renderMediaBrowser() {
  const media = state.document.media;
  const url = cacheBustUrl(media.url);
  const player = media.mediaType === "audio"
    ? '<audio controls preload="metadata" src="' + esc(url) + '"></audio>'
    : '<video controls preload="metadata" src="' + esc(url) + '"></video>';
  els.mediaBrowser.innerHTML = '<div class="media-hero"><h3>' + esc(media.title || media.name) + '</h3><p class="media-note">' +
    esc((media.mediaType || "media").toUpperCase() + " · " + formatBytes(media.size) + " · " + basename(media.path)) +
    '</p><div class="selection-preview">' + player + '</div><p class="media-note">Playback depends on browser codec support. The original extracted file is also available from the inspector.</p></div>';
}

function metadataRows(record) {
  if (!record) return [];
  const asset = record.asset || {};
  const rows = [
    ["Type", record.kind],
    ["Component", record.component?.type],
    ["Runtime ID", record.component ? runtimeIdFor(record.component, asset) : asset.id],
    ["Name", asset.name],
    ["Dimensions", asset.width && asset.height ? asset.width + " × " + asset.height : null],
    ["Rectangle", asset.rect ? asset.rect.x + ", " + asset.rect.y + " · " + asset.rect.width + " × " + asset.rect.height : null],
    ["Layer Z", asset.z],
    ["Frames / phases", asset.frameCount ?? asset.phaseCount],
    ["Duration", asset.durationSeconds ? asset.durationSeconds + " s" : null],
    ["Visible", asset.initiallyVisible],
    ["Draggable", asset.draggable],
    ["Geometry", asset.geometryConfidence],
    ["Offset", asset.offset ?? asset.instanceOffset ?? asset.entryOffset],
    ["Size", asset.size != null ? formatBytes(asset.size) : null],
    ["File", basename(asset.path)],
  ];
  return rows.filter((row) => row[1] !== null && row[1] !== undefined && row[1] !== "");
}

function renderSelection() {
  const record = state.selectedRecord;
  els.selectionPreview.innerHTML = "";
  if (!record) {
    els.selectionInfo.innerHTML = '<div class="compact-empty">Select an asset or component.</div>';
    return;
  }
  if (record.previewUrl && !["audio", "video", "script"].includes(record.type) && !["audio", "video", "script"].includes(record.kind)) {
    els.selectionPreview.innerHTML = '<img src="' + esc(cacheBustUrl(record.previewUrl)) + '" alt="' + esc(record.title) + '">';
  } else if (record.url && (record.type === "audio" || record.kind === "audio")) {
    els.selectionPreview.innerHTML = '<audio controls preload="metadata" src="' + esc(cacheBustUrl(record.url)) + '"></audio>';
  } else if (record.url && (record.kind === "video" || record.type === "video")) {
    els.selectionPreview.innerHTML = '<video controls preload="metadata" src="' + esc(cacheBustUrl(record.url)) + '"></video>';
  }
  const rows = metadataRows(record).map(([key, value]) => '<tr><th>' + esc(key) + '</th><td>' + esc(value) + '</td></tr>').join("");
  const links = record.url
    ? '<div class="selection-links"><a class="button-link" href="' + esc(record.url) + '" target="_blank" rel="noreferrer">Open asset</a></div>'
    : "";
  els.selectionInfo.innerHTML = '<h2>' + esc(record.title) + '</h2><table class="metadata-table">' + rows + '</table>' + links;
  document.querySelectorAll("[data-record-key]").forEach((card) => card.classList.toggle("selected", card.dataset.recordKey === record.key));
  document.querySelectorAll("[data-inspect-key]").forEach((layer) => {
    if (layer.classList.contains("bounds")) return;
    layer.style.outline = layer.dataset.inspectKey === record.key ? "2px solid #67d5b5" : "";
    layer.style.outlineOffset = layer.dataset.inspectKey === record.key ? "-1px" : "";
  });
}

function selectRecord(key) {
  const record = state.records.find((item) => item.key === key);
  if (!record) return;
  state.selectedRecord = record;
  renderSelection();
  document.querySelector(".inspector-tab[data-panel='inspect']")?.click();
}

function componentRecord(component) {
  return {
    key: "component:" + component.id,
    title: component.type,
    kind: "component",
    type: "component",
    asset: {
      id: component.index,
      rect: component.rect,
      z: component.z,
      geometryConfidence: component.geometryConfidence,
      size: component.privateSize,
      offset: component.sourceOffset,
      name: component.id,
    },
    component,
  };
}

function renderComponents(scene) {
  const components = scene.components || [];
  els.componentCount.textContent = String(components.length);
  els.componentList.innerHTML = components.map((component) => {
    const assets = component.assets?.length || 0;
    const audio = component.audio?.length || 0;
    const selected = state.componentFilter === component.id;
    return '<button type="button" class="component-item ' + (selected ? "selected" : "") + '" data-component-id="' + esc(component.id) + '">' +
      '<strong>' + esc(component.index) + '. ' + esc(component.type) + '</strong>' +
      '<small>' + assets + " image · " + audio + " audio · " + (component.hitboxes?.length || 0) + " hitbox<br>" + esc(component.geometryConfidence) + " · offset " + esc(component.sourceOffset) + '</small>' +
    '</button>';
  }).join("") || '<div class="compact-empty">No component wrappers.</div>';
}

function renderAudio(scene) {
  const audioAssets = [];
  for (const component of scene.components || []) {
    for (const asset of component.audio || []) audioAssets.push({ component, asset });
  }
  if (!audioAssets.length) {
    els.audioList.innerHTML = '<div class="compact-empty">No embedded audio.</div>';
    return;
  }
  const rows = audioAssets.slice(0, 160).map(({ component, asset }, index) => (
    '<div class="audio-row"><div class="audio-row-label"><span>' + (index + 1) + '. ' + esc(component.type) + " #" + esc(runtimeIdFor(component, asset)) + '</span><span>' + esc(asset.durationSeconds ? asset.durationSeconds + "s" : asset.kind) + '</span></div>' +
    '<audio controls preload="none" src="' + esc(cacheBustUrl(asset.url)) + '" data-runtime-namespace="' + (state.mode === "group" ? "Group" : "Page") + '" data-component-type="' + esc(component.type) + '" data-asset-id="' + esc(asset.id) + '" data-runtime-id="' + esc(runtimeIdFor(component, asset)) + '"></audio></div>'
  )).join("");
  els.audioList.innerHTML = '<details class="audio-pack"><summary>' + audioAssets.length + ' embedded audio asset(s)</summary><div class="audio-grid">' + rows + '</div></details>';
}

function renderScript(scene) {
  const script = state.mode === "scene" ? (scene.script || {}) : {};
  const handlers = script.handlers || [];
  els.handlerSelect.innerHTML = handlers.map((handler) => '<option value="' + esc(handler.name) + '">' + esc(handler.signature) + '</option>').join("");
  [els.handlerSelect, els.handlerArgs, els.runHandler, els.openPage, els.resetRuntime].forEach((control) => { control.disabled = state.mode !== "scene" || !state.document; });
  els.handlerSelect.disabled = state.mode !== "scene" || handlers.length === 0;
  els.handlerArgs.disabled = els.handlerSelect.disabled;
  els.runHandler.disabled = els.handlerSelect.disabled;
  els.scriptInfo.innerHTML = state.mode !== "scene"
    ? '<div>No page script in this archive type.</div>'
    : '<div>' + (script.url ? '<a href="' + esc(script.url) + '" target="_blank" rel="noreferrer">Open recovered script.txt</a>' : "No script file") + '</div><div>' + handlers.length + ' handler(s)</div>' +
      handlers.slice(0, 60).map((handler) => '<div>' + esc(handler.signature) + '</div>').join("");
}

function renderWarnings(scene) {
  const warnings = scene.warnings || [];
  els.warnings.innerHTML = warnings.map((warning) => '<div>⚠ ' + esc(warning) + '</div>').join("");
}

function renderRaw(scene) {
  els.rawJson.textContent = JSON.stringify(scene, null, 2);
  const entry = currentEntry();
  if (state.mode === "loose") {
    els.openJson.removeAttribute("href");
    els.openJson.style.display = "none";
  } else {
    els.openJson.href = cacheBustUrl(entry.url);
    els.openJson.style.display = "";
  }
}

function renderDocumentHeader() {
  const entry = currentEntry();
  const scene = state.document;
  const source = scene.sourceBdf || scene.sourceGrp || scene.sourceMedia || "";
  els.documentType.textContent = state.mode === "scene" ? "BDF scene" : state.mode === "group" ? "GRP library" : "Loose media";
  els.documentTitle.textContent = scene.title || scene.id;
  els.documentMeta.textContent = basename(source) + (entry ? " · " + entrySubtitle(entry) : "");
  document.title = (scene.title || scene.id) + " — GraphBoard Archive";
}

function render() {
  const scene = state.document;
  renderDocumentHeader();
  els.stageWrap.hidden = state.mode === "loose";
  els.mediaBrowser.hidden = state.mode !== "loose";
  if (state.mode === "loose") {
    renderMediaBrowser();
    els.stageDimensions.textContent = "media file";
    els.pointerPosition.textContent = "";
    els.layerCount.textContent = "";
  } else {
    renderStage(scene);
  }
  renderComponents(scene);
  renderAudio(scene);
  renderScript(scene);
  renderWarnings(scene);
  renderRaw(scene);
  renderAssetGrid();
  runtime.reset(scene);
  if (state.mode === "scene") setTimeout(() => runtime.executeHandler("OnOpenPage"), 0);
}

function switchInspector(panel) {
  document.querySelectorAll(".inspector-tab").forEach((tab) => tab.classList.toggle("active", tab.dataset.panel === panel));
  document.querySelectorAll(".inspector-panel").forEach((item) => item.classList.toggle("active", item.id === panel + "Panel"));
}

function bindUi() {
  document.querySelectorAll(".mode-tab").forEach((button) => button.addEventListener("click", () => setMode(button.dataset.mode)));
  document.querySelectorAll(".inspector-tab").forEach((button) => button.addEventListener("click", () => switchInspector(button.dataset.panel)));
  els.collectionList.addEventListener("click", (event) => {
    const button = event.target.closest("[data-entry-index]");
    if (button) loadDocument(Number(button.dataset.entryIndex));
  });
  els.select.addEventListener("change", () => loadDocument(Number(els.select.value)));
  els.search.addEventListener("input", renderLibrary);
  els.prev.addEventListener("click", () => loadDocument(state.selected[state.mode] - 1));
  els.next.addEventListener("click", () => loadDocument(state.selected[state.mode] + 1));
  els.assetGrid.addEventListener("click", (event) => {
    const button = event.target.closest("[data-record-key]");
    if (!button) return;
    const record = state.records.find((item) => item.key === button.dataset.recordKey);
    if (state.mode === "loose" && record?.asset?.id !== state.document.media.id) {
      const index = entriesForMode().findIndex((entry) => entry.id === record.asset.id);
      if (index >= 0) loadDocument(index);
      return;
    }
    selectRecord(button.dataset.recordKey);
  });
  els.assetFilter.addEventListener("change", renderAssetGrid);
  els.assetSearch.addEventListener("input", renderAssetGrid);
  els.componentList.addEventListener("click", (event) => {
    const button = event.target.closest("[data-component-id]");
    if (!button) return;
    const id = button.dataset.componentId;
    state.componentFilter = state.componentFilter === id ? null : id;
    const component = state.document.components.find((item) => item.id === id);
    if (component) state.selectedRecord = componentRecord(component);
    renderComponents(state.document);
    renderAssetGrid();
    if (component) {
      state.selectedRecord = componentRecord(component);
      renderSelection();
    }
  });
  els.fit.addEventListener("click", () => setZoom("fit"));
  els.actual.addEventListener("click", () => setZoom("actual", 1));
  els.zoomOut.addEventListener("click", () => setZoom("custom", state.zoom.value - .1));
  els.zoomIn.addEventListener("click", () => setZoom("custom", state.zoom.value + .1));
  els.openPage.addEventListener("click", () => runtime.executeHandler("OnOpenPage"));
  els.resetRuntime.addEventListener("click", render);
  els.runHandler.addEventListener("click", () => {
    const args = els.handlerArgs.value.split(",").map((item) => item.trim()).filter(Boolean).map((item) => (/^-?\d+$/.test(item) ? Number(item) : item));
    runtime.executeHandler(els.handlerSelect.value, args);
  });
  [els.debug, els.unknown, els.background].forEach((input) => input.addEventListener("change", render));
  els.copyRaw.addEventListener("click", async () => {
    try {
      await navigator.clipboard.writeText(els.rawJson.textContent);
      const old = els.copyRaw.textContent;
      els.copyRaw.textContent = "Copied";
      setTimeout(() => { els.copyRaw.textContent = old; }, 1000);
    } catch {
      els.copyRaw.textContent = "Copy failed";
    }
  });

  els.stage.addEventListener("mousedown", (event) => {
    if (event.button !== 0 || state.mode !== "scene") return;
    const point = stagePointFromEvent(event);
    runtime.executeHandlerIfPresent("OnLButtonDown", [point.x, point.y]);
  }, true);
  els.stage.addEventListener("mouseup", (event) => {
    if (event.button !== 0 || state.mode !== "scene") return;
    const point = stagePointFromEvent(event);
    runtime.executeHandlerIfPresent("OnLButtonUp", [point.x, point.y]);
    runtime.endPointerInteraction(point);
  }, true);
  els.stage.addEventListener("contextmenu", (event) => {
    if (state.mode !== "scene") return;
    event.preventDefault();
    const point = stagePointFromEvent(event);
    runtime.executeHandlerIfPresent("OnRButtonDown", [point.x, point.y]);
  }, true);
  els.stage.addEventListener("mousemove", (event) => {
    const point = stagePointFromEvent(event);
    els.pointerPosition.textContent = "x " + point.x + ", y " + point.y;
    if (state.mode === "scene") {
      runtime.updatePointerInteraction(point);
      runtime.handleStagePointer(point);
    }
  });
  els.stage.addEventListener("mouseleave", () => {
    els.pointerPosition.textContent = "x —, y —";
    if (state.mode === "scene") runtime.handleStagePointer(null);
  });
  window.addEventListener("keydown", (event) => {
    if (event.key === "/" && !["INPUT", "SELECT", "TEXTAREA"].includes(document.activeElement?.tagName)) {
      event.preventDefault();
      els.search.focus();
    } else if (event.altKey && event.key === "ArrowLeft") {
      event.preventDefault();
      els.prev.click();
    } else if (event.altKey && event.key === "ArrowRight") {
      event.preventDefault();
      els.next.click();
    }
  });
  new ResizeObserver(() => {
    if (state.zoom.mode === "fit") applyZoom();
  }).observe(els.stageWrap);
}

async function boot() {
  const [index, groupIndex, media] = await Promise.all([
    getJson("scenes/index.json"),
    getJson("groups/index.json").catch(() => ({ groups: [] })),
    getJson("assets/index.json").catch(() => ({ assets: [] })),
  ]);
  state.index = index;
  state.groupIndex = groupIndex;
  state.media = media;
  state.groups = await Promise.all((groupIndex.groups || []).map((group) => getJson(group.url)));
  runtime.setGroups(state.groups);
  const validation = index.validation || {};
  const embedded = Number(validation.totalAssets || 0) + Number(validation.totalGroupAssets || 0);
  state.summary = index.scenes.length + " BDF · " + state.groups.length + " GRP · " + embedded.toLocaleString() + " embedded assets";
  els.summary.textContent = state.summary;
  els.sceneCount.textContent = String(index.scenes.length);
  els.groupCount.textContent = String(state.groups.length);
  els.mediaCount.textContent = String(media.assets?.length || 0);
  bindUi();

  const params = new URLSearchParams(location.search);
  let mode = "scene";
  let wanted = params.get("scene");
  if (params.has("group")) { mode = "group"; wanted = params.get("group"); }
  if (params.has("media")) { mode = "loose"; wanted = params.get("media"); }
  const entries = entriesForMode(mode);
  const selected = wanted ? entries.findIndex((entry) => String(entry.id).toLowerCase() === String(wanted).toLowerCase()) : 0;
  state.selected[mode] = selected >= 0 ? selected : 0;
  setMode(mode, false);
  await loadDocument(state.selected[mode], { updateUrl: false });
}

boot().catch((error) => {
  els.summary.textContent = "Failed to load archive index";
  els.warnings.textContent = error.stack || String(error);
});
