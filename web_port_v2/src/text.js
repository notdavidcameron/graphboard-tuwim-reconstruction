// TextHolder pass: the indexed scene comes out of the portable renderer, but
// text uses the platform font — same split gbgame makes with DrawTextW. This
// draws the shim's visible text items with DT_WORDBREAK-style wrapping.

const FONT = '18px "Segoe UI", Tahoma, sans-serif';
const COLOR = "rgb(24, 24, 24)";
const LINE_HEIGHT = 22;

/**
 * @param {CanvasRenderingContext2D} ctx - canvas at native page resolution
 * @param {Array<{l:number,t:number,r:number,b:number,text:string}>} items
 */
export function drawTextItems(ctx, items) {
  if (!items.length) return;
  ctx.save();
  ctx.font = FONT;
  ctx.fillStyle = COLOR;
  ctx.textBaseline = "top";
  for (const item of items) {
    const width = item.r - item.l;
    let y = item.t;
    for (const paragraph of item.text.split(/\r\n|\r|\n/)) {
      if (y >= item.b) break;
      if (paragraph === "") {
        y += LINE_HEIGHT;
        continue;
      }
      // Greedy word wrap, like DrawText's DT_WORDBREAK.
      let line = "";
      for (const word of paragraph.split(" ")) {
        const candidate = line ? line + " " + word : word;
        if (line && ctx.measureText(candidate).width > width) {
          ctx.fillText(line, item.l, y);
          y += LINE_HEIGHT;
          if (y >= item.b) { line = ""; break; }
          line = word;
        } else {
          line = candidate;
        }
      }
      if (line && y < item.b) {
        ctx.fillText(line, item.l, y);
        y += LINE_HEIGHT;
      }
    }
  }
  ctx.restore();
}
