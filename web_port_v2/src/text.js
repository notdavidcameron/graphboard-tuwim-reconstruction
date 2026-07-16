// TextHolder canvas pass. Besides the holder's manual offsets, synchronized
// narration follows the active line and highlights it, approximating the old
// FontControl karaoke display when only the WAV/EXS stream survives.

const FONT = '700 17px "Segoe UI", Tahoma, sans-serif';
const LINE_HEIGHT = 22;

function wrapLines(ctx, text, width) {
  const lines = [];
  for (const paragraph of text.split(/\r\n|\r|\n/)) {
    if (!paragraph) {
      lines.push("");
      continue;
    }
    let line = "";
    for (const word of paragraph.split(" ")) {
      const candidate = line ? `${line} ${word}` : word;
      if (line && ctx.measureText(candidate).width > width) {
        lines.push(line);
        line = word;
      } else {
        line = candidate;
      }
    }
    lines.push(line);
  }
  return lines;
}

function activeLineForProgress(lines, progress) {
  if (!(progress >= 0) || !lines.length) return -1;
  const weights = lines.map((line) => Math.max(1, line.trim().length));
  const total = weights.reduce((sum, value) => sum + value, 0);
  let cursor = Math.min(0.999999, progress) * total;
  for (let i = 0; i < weights.length; i++) {
    if (cursor < weights[i]) return i;
    cursor -= weights[i];
  }
  return lines.length - 1;
}

/**
 * @param {CanvasRenderingContext2D} ctx
 * @param {Array<object>} items
 * @param {(key:string)=>number} progressForKey returns 0..1, or -1 when idle
 */
export function drawTextItems(ctx, items, progressForKey = () => -1) {
  if (!items.length) return;
  ctx.save();
  ctx.font = FONT;
  ctx.textBaseline = "top";

  for (const item of items) {
    const width = Math.max(1, item.r - item.l);
    const lines = wrapLines(ctx, item.text, width);
    const isPoem = item.text.length > 160;
    const progress = (item.playing || isPoem)
      ? progressForKey(`Text_Holder/${item.id}`, item)
      : -1;
    const active = activeLineForProgress(lines, progress);
    const viewportHeight = Math.max(1, item.b - item.t);
    let offset = Number(item.offsetY) || 0;

    // Keep the narrated line near the middle of the text viewport. Manual
    // wheel/stanza offsets remain authoritative while narration is idle.
    if (active >= 0) {
      const desired = Math.floor((viewportHeight - LINE_HEIGHT) / 2) - active * LINE_HEIGHT;
      const minimum = Math.min(0, viewportHeight - lines.length * LINE_HEIGHT);
      offset = Math.max(minimum, Math.min(0, desired));
    }

    // A dark veil and bright lettering remain legible over both pale and dark
    // scanned artwork without hiding the scene completely.
    ctx.fillStyle = "rgba(0, 0, 0, 0.58)";
    ctx.fillRect(item.l - 8, item.t - 6, width + 16, viewportHeight + 12);
    ctx.save();
    ctx.beginPath();
    ctx.rect(item.l, item.t, width, viewportHeight);
    ctx.clip();
    // The serialized FontControl colour is frequently bright green, which is
    // faithful to the old palette but unreadable over the scanned artwork.
    // Keep the karaoke highlight distinct and use a stable near-black text
    // colour for the ordinary poem lines.
    const color = [255, 255, 255];
    for (let i = 0; i < lines.length; i++) {
      const y = item.t + offset + i * LINE_HEIGHT;
      if (y + LINE_HEIGHT <= item.t || y >= item.b) continue;
      if (i === active) {
        ctx.fillStyle = "rgba(255, 224, 72, 0.86)";
        ctx.fillRect(item.l - 2, y - 1, width + 4, LINE_HEIGHT);
        ctx.fillStyle = "rgb(20, 20, 20)";
      } else {
        ctx.fillStyle = `rgb(${color[0]}, ${color[1]}, ${color[2]})`;
      }
      ctx.lineWidth = 2;
      ctx.strokeStyle = "rgba(0, 0, 0, 0.75)";
      ctx.strokeText(lines[i], item.l, y);
      ctx.fillText(lines[i], item.l, y);
    }
    ctx.restore();
  }
  ctx.restore();
}
