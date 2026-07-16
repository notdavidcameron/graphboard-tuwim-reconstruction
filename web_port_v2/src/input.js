// Pointer + keyboard wiring: maps CSS-scaled canvas coordinates to native page
// coordinates and forwards to the shim's recovered input entry points.

export function attachInput(canvas, api) {
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
    // e.keyCode matches Win32 VK_* for letters, digits, arrows, space, enter —
    // the codes the recovered scripts actually test.
    api.keyDown(e.keyCode);
    if (["ArrowUp", "ArrowDown", "ArrowLeft", "ArrowRight", " "].includes(e.key)) {
      e.preventDefault();
    }
  });
  window.addEventListener("keyup", (e) => {
    api.keyUp(e.keyCode);
    if (["ArrowUp", "ArrowDown", "ArrowLeft", "ArrowRight", " "].includes(e.key)) {
      e.preventDefault();
    }
  });
}
