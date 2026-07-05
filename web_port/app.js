const state = {
  index: null,
  scene: null,
  selected: 0,
};
const CACHE_VERSION = "24";
window.GRAPHBOARD_CACHE_VERSION = CACHE_VERSION;

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
window.graphBoardRuntime = runtime;
runtime.onLoadPage = (bdfName) => {
  const clean = String(bdfName || "").replace(/^.*[\\/]/, "").replace(/\.bdf$/i, "").toLowerCase();
  const index = state.index.scenes.findIndex((scene) => (
    scene.id.toLowerCase() === clean ||
    String(scene.sourceBdf || "").replace(/^.*[\\/]/, "").replace(/\.bdf$/i, "").toLowerCase() === clean
  ));
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
  const response = await fetch(cacheUrl(url));
  if (!response.ok) {
    throw new Error(`${url}: ${response.status}`);
  }
  return response.json();
}

function cacheUrl(url) {
  if (!url || /^(?:data:|blob:)/i.test(url)) return url;
  const separator = url.includes("?") ? "&" : "?";
  return `${url}${separator}v=${CACHE_VERSION}`;
}

function sceneEntry() {
  return state.index.scenes[state.selected];
}

async function loadScene(selected) {
  state.selected = Math.max(0, Math.min(selected, state.index.scenes.length - 1));
  els.select.value = String(state.selected);
  const entry = sceneEntry();
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
  if (!asset) return component.index ?? 0;
  const id = Number(asset.id ?? 0);
  if (component.type === "Transparent_Video_Holder" || component.type === "Sprite_Holder") {
    return id > 0 && id % 2 === 0 ? id / 2 : id;
  }
  return id;
}

function initialAssetVisible(component, asset, isPrimary, hasUnknownGeometry) {
  if (!component.visible) return false;
  if (component.type === "MultiBitmap") {
    return asset.kind === "multibitmap_bmp" && Number(asset.id ?? -1) === 0;
  }
  if (component.type === "Transparent_Video_Holder" && asset.geometryConfidence === "serialized_entry") {
    return false;
  }
  const assetHasGeometry = asset.geometryConfidence && asset.geometryConfidence !== "unknown";
  return isPrimary || els.unknown.checked || !hasUnknownGeometry || assetHasGeometry;
}

function bindRuntimeEvents(element, component, runtimeId) {
  element.dataset.componentType = component.type;
  element.dataset.runtimeId = String(runtimeId);
  element.addEventListener("mouseenter", () => runtime.dispatchComponentEvent(component.type, "MouseMoveIn", runtimeId));
  element.addEventListener("mouseleave", () => runtime.dispatchComponentEvent(component.type, "MouseMoveOut", runtimeId));
  element.addEventListener("mousedown", (event) => {
    if (event.button === 0) {
      runtime.dispatchComponentEvent(
        component.type,
        component.type === "HotSpot_Holder" ? "LeftButtonClickOn" : "MouseClickOnDown",
        runtimeId
      );
    }
  });
  element.addEventListener("mouseup", (event) => {
    if (event.button === 0) {
      runtime.dispatchComponentEvent(
        component.type,
        component.type === "HotSpot_Holder" ? "LeftButtonClickOnUp" : "MouseClickOnUp",
        runtimeId
      );
    }
  });
  element.addEventListener("click", (event) => {
    event.stopPropagation();
    if (component.type === "Text_Holder") {
      runtime.dispatchComponentEvent(component.type, "ClickOnText", runtimeId);
    }
  });
}

function renderHotspotButton(component, hotspot) {
  const rect = hotspot.rect || { x: 0, y: 0, width: 1, height: 1 };
  const runtimeId = Number(hotspot.id ?? hotspot.index ?? component.index ?? 0);
  const layerZ = Number.isFinite(Number(hotspot.z)) ? Number(hotspot.z) : Number(component.z ?? 0);
  const button = document.createElement("button");
  button.type = "button";
  button.className = "hitbox hotspot";
  button.style.left = `${rect.x}px`;
  button.style.top = `${rect.y}px`;
  button.style.width = `${Math.max(rect.width, 8)}px`;
  button.style.height = `${Math.max(rect.height, 8)}px`;
  button.style.zIndex = String(900 + layerZ * 1000 + runtimeId);
  button.title = `${component.type} ${runtimeId}${hotspot.name ? ` ${hotspot.name}` : ""}`;
  button.dataset.hotspotId = String(runtimeId);
  bindRuntimeEvents(button, component, runtimeId);
  button.classList.toggle("runtime-disabled", hotspot.enabled === false);
  els.stage.appendChild(button);

  if (els.debug.checked) {
    const box = document.createElement("div");
    box.className = "bounds hotspot";
    box.style.left = `${rect.x}px`;
    box.style.top = `${rect.y}px`;
    box.style.width = `${Math.max(rect.width, 18)}px`;
    box.style.height = `${Math.max(rect.height, 14)}px`;
    box.style.zIndex = String(2000 + layerZ * 1000 + runtimeId);
    box.textContent = `H${runtimeId}`;
    els.stage.appendChild(box);
  }
}

function renderStage(scene) {
  els.stage.replaceChildren();
  setStageSize(scene);

  if (els.background.checked && scene.background?.bmpUrl) {
    const img = document.createElement("img");
    img.className = "stage-bg";
    img.src = cacheUrl(scene.background.bmpUrl);
    img.alt = "";
    els.stage.appendChild(img);
  } else {
    const colorIndex = scene.background?.colorIndex ?? 0;
    els.stage.style.background = scene.background?.color || `hsl(${(colorIndex * 31) % 360} 28% 18%)`;
  }

  const components = [...(scene.components || [])].sort((a, b) => (a.z ?? 0) - (b.z ?? 0));
  for (const component of components) {
    const assets = component.assets || [];
    const hotspots = component.hotspots || [];
    const hasUnknownGeometry = component.geometryConfidence === "unknown";
    for (const asset of assets) {
      if (!asset.url) continue;
      const rect = asset.rect || component.rect || { x: 0, y: 0, width: 1, height: 1 };
      const layerZ = Number.isFinite(Number(asset.z)) ? Number(asset.z) : Number(component.z ?? 0);
      const assetOrder = Number.isFinite(Number(asset.id)) ? Number(asset.id) : 0;
      const assetHasGeometry = asset.geometryConfidence && asset.geometryConfidence !== "unknown";
      const img = document.createElement("img");
      img.className = "layer";
      img.src = cacheUrl(asset.url);
      img.alt = `${component.id}:${asset.id ?? ""}`;
      img.dataset.originalSrc = cacheUrl(asset.url);
      img.dataset.componentType = component.type;
      img.dataset.assetId = String(asset.id ?? "");
      img.dataset.runtimeId = String(runtimeIdFor(component, asset));
      img.dataset.runtimeKey = `${component.type}:${asset.id ?? ""}`;
      img.dataset.runtimeLookupKey = `${component.type}:${runtimeIdFor(component, asset)}`;
      img.style.left = `${rect.x}px`;
      img.style.top = `${rect.y}px`;
      img.style.width = `${asset.width || rect.width}px`;
      img.style.height = `${asset.height || rect.height}px`;
      img.style.zIndex = String(1000 + layerZ * 1000 + assetOrder);
      const isPrimary = component.primaryAssetUrl === asset.url;
      const shouldShow = initialAssetVisible(component, asset, isPrimary, hasUnknownGeometry);
      img.classList.toggle("runtime-hidden", !shouldShow);
      bindRuntimeEvents(img, component, runtimeIdFor(component, asset));
      els.stage.appendChild(img);

      if (els.debug.checked && assetHasGeometry) {
        const box = document.createElement("div");
        box.className = "bounds asset";
        box.style.left = `${rect.x}px`;
        box.style.top = `${rect.y}px`;
        box.style.width = `${Math.max(rect.width, 18)}px`;
        box.style.height = `${Math.max(rect.height, 14)}px`;
        box.style.zIndex = String(2000 + layerZ * 1000 + assetOrder);
        box.textContent = `${component.index}:${asset.id ?? ""}`;
        els.stage.appendChild(box);
      }
    }

    for (const hotspot of hotspots) {
      renderHotspotButton(component, hotspot);
    }

    if (!hotspots.length && (els.unknown.checked || !hasUnknownGeometry || component.type === "HotSpot_Holder")) {
      const rect = component.rect || { x: 0, y: 0, width: 1, height: 1 };
      const hitbox = document.createElement("button");
      hitbox.type = "button";
      hitbox.className = "hitbox";
      hitbox.style.left = `${rect.x}px`;
      hitbox.style.top = `${rect.y}px`;
      hitbox.style.width = `${Math.max(rect.width, 8)}px`;
      hitbox.style.height = `${Math.max(rect.height, 8)}px`;
      hitbox.style.zIndex = String(component.type === "HotSpot_Holder" ? 900 + (component.z ?? 0) : 1 + (component.z ?? 0));
      hitbox.title = `${component.type} ${component.index}`;
      bindRuntimeEvents(hitbox, component, component.index ?? 0);
      els.stage.appendChild(hitbox);
    }

    if (els.debug.checked && (els.unknown.checked || !hasUnknownGeometry || component.visible)) {
      const rect = component.rect || { x: 0, y: 0, width: 1, height: 1 };
      const box = document.createElement("div");
      box.className = `bounds ${component.geometryConfidence === "unknown" ? "unknown" : ""}`;
      box.style.left = `${rect.x}px`;
      box.style.top = `${rect.y}px`;
      box.style.width = `${Math.max(rect.width, 24)}px`;
      box.style.height = `${Math.max(rect.height, 18)}px`;
      box.style.zIndex = String(1000 + (component.z ?? 0));
      box.textContent = `${component.index}: ${component.type} (${component.geometryConfidence})`;
      els.stage.appendChild(box);
    }
  }
}

function renderComponents(scene) {
  const rows = (scene.components || []).map((component) => {
    const assets = component.assets?.length || 0;
    const audio = component.audio?.length || 0;
    const hotspots = component.hotspots?.length || 0;
    const rect = component.rect || {};
    return `
      <div class="item">
        <strong>${esc(component.index)}. ${esc(component.type)}</strong>
        <small>${esc(component.geometryConfidence)} rect ${esc(rect.x)},${esc(rect.y)} ${esc(rect.width)}x${esc(rect.height)}</small>
        <small>${assets} image asset(s), ${audio} audio asset(s), ${hotspots} hotspot(s), offset ${esc(component.sourceOffset)}</small>
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
      <audio controls preload="none" src="${esc(cacheUrl(asset.url))}" data-component-type="${esc(component.type)}" data-asset-id="${esc(asset.id)}" data-runtime-id="${esc(runtimeIdFor(component, asset))}" data-runtime-key="${esc(component.type)}:${esc(asset.id)}" data-duration-seconds="${esc(asset.durationSeconds || "")}"></audio>
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
  els.summary.textContent = `${state.index.scenes.length} pages, ${state.index.validation.totalComponents} components, ${state.index.validation.totalAssets} assets`;
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
    if (event.button !== 0 || event.target !== els.stage) return;
    const rect = els.stage.getBoundingClientRect();
    runtime.markUserActivated();
    runtime.executeHandler("OnLButtonDown", [Math.round(event.clientX - rect.left), Math.round(event.clientY - rect.top)]);
  });
  els.stage.addEventListener("mouseup", (event) => {
    if (event.button !== 0 || event.target !== els.stage) return;
    const rect = els.stage.getBoundingClientRect();
    runtime.markUserActivated();
    runtime.executeHandler("OnLButtonUp", [Math.round(event.clientX - rect.left), Math.round(event.clientY - rect.top)]);
  });
  for (const input of [els.debug, els.unknown, els.background]) {
    input.addEventListener("change", render);
  }

  const params = new URLSearchParams(location.search);
  const wanted = params.get("scene");
  const selected = wanted
    ? Math.max(0, state.index.scenes.findIndex((scene) => scene.id.toLowerCase() === wanted.toLowerCase()))
    : 0;
  await loadScene(selected < 0 ? 0 : selected);
}

boot().catch((error) => {
  els.summary.textContent = "Failed to load scene index";
  els.warnings.textContent = error.stack || String(error);
});
