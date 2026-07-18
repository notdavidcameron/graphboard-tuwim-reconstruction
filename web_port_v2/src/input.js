// Pointer + keyboard wiring: maps CSS-scaled canvas coordinates to native page
// coordinates and forwards to the shim's recovered input entry points.

export function attachInput(canvas, api) {
  // `keyCode` is the value used by the recovered Win32 scripts, but it is
  // deprecated and can be zero in some browser/event combinations. Keep the
  // physical-key fallback for the keys that are especially easy for a page
  // to consume (Tab, Shift, and punctuation).
  const codeToVirtualKey = {
    Tab: 9,
    ShiftLeft: 16,
    ShiftRight: 16,
    // These scripts use ASCII punctuation (44/46), unlike the OEM virtual
    // keys (188/190) returned by browser keyCode for the same physical keys.
    Comma: 44,
    Period: 46,
    Slash: 47,
    BracketLeft: 91,
    BracketRight: 93,
  };

  const virtualKey = (event) => {
    if (event.code === "Comma") return 44;
    if (event.code === "Period") return 46;
    if (event.code === "Slash") return 47;
    const legacyCode = Number(event.keyCode || event.which);
    if (legacyCode === 188) return 44;
    if (legacyCode === 190) return 46;
    if (legacyCode === 191) return 47;
    return legacyCode || codeToVirtualKey[event.code] || 0;
  };

  const shouldPreventBrowserDefault = (event) => {
    // Leave browser/application shortcuts such as Ctrl+L and Alt+Left alone.
    // Plain keys, including Shift by itself, belong to the game once it is
    // running.
    if (event.ctrlKey || event.altKey || event.metaKey) return false;
    return [
      "Tab", "ShiftLeft", "ShiftRight", "Comma", "Period", "Slash",
      "BracketLeft", "BracketRight",
      "ArrowUp", "ArrowDown", "ArrowLeft", "ArrowRight", "Space",
    ].includes(event.code);
  };

  const toPage = (e) => {
    const rect = canvas.getBoundingClientRect();
    return {
      x: Math.round((e.clientX - rect.left) * (canvas.width / rect.width)),
      y: Math.round((e.clientY - rect.top) * (canvas.height / rect.height)),
    };
  };

  canvas.addEventListener("pointermove", (e) => {
    const p = toPage(e);
    api.mouseMove(p.x, p.y);
  });
  canvas.addEventListener("pointerleave", () => api.mouseLeave());
  canvas.addEventListener("pointerdown", (e) => {
    canvas.setPointerCapture(e.pointerId);
    const p = toPage(e);
    if (e.button === 2) api.mouseRDown(p.x, p.y);
    else api.mouseDown(p.x, p.y);
  });
  canvas.addEventListener("pointerup", (e) => {
    canvas.releasePointerCapture(e.pointerId);
    const p = toPage(e);
    if (e.button !== 2) api.mouseUp(p.x, p.y);
  });
  canvas.addEventListener("contextmenu", (e) => e.preventDefault());

  window.addEventListener("keydown", (e) => {
    if (e.isComposing) return;
    const vk = virtualKey(e);
    if (!vk) return;
    api.keyDown(vk);
    if (shouldPreventBrowserDefault(e)) e.preventDefault();
  }, { capture: true });
  window.addEventListener("keyup", (e) => {
    if (e.isComposing) return;
    const vk = virtualKey(e);
    if (!vk) return;
    api.keyUp(vk);
    if (shouldPreventBrowserDefault(e)) e.preventDefault();
  }, { capture: true });
}
