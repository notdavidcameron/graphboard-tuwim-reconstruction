// Corpus smoke test: run OnOpenPage (and a synthetic OnTimer/OnLButtonDown)
// for every exported scene under a minimal DOM stub. Catches parser crashes.
const fs = require("fs");
const path = require("path");
const ROOT = path.join(__dirname, "..");

function makeElement(tag) {
  return {
    tagName: String(tag || "div").toUpperCase(),
    dataset: {},
    style: {},
    classList: { add() {}, remove() {}, toggle() {}, contains: () => false },
    children: [],
    appendChild(c) { this.children.push(c); },
    querySelectorAll: () => [],
    querySelector: () => null,
    addEventListener() {},
    setAttribute() {},
    getAttribute: () => "",
    getBoundingClientRect: () => ({ left: 0, top: 0, width: 10, height: 10 }),
    remove() {},
    isConnected: true,
    offsetWidth: 10,
    offsetHeight: 10,
    complete: false,
    textContent: "",
  };
}
global.window = {};
global.document = {
  createElement: makeElement,
  querySelectorAll: () => [],
};
global.Audio = function () { return Object.assign(makeElement("audio"), { play: () => Promise.resolve(), pause() {} }); };

const src = fs.readFileSync(path.join(ROOT, "runtime.js"), "utf8");
eval(src + "\nglobal.GraphBoardRuntime = GraphBoardRuntime;");

const stage = Object.assign(makeElement("div"), { clientWidth: 640, clientHeight: 480 });
let crashed = 0, ran = 0, skipped = 0;
for (const file of fs.readdirSync(path.join(ROOT, "scenes"))) {
  if (!file.endsWith(".json")) continue;
  const scene = JSON.parse(fs.readFileSync(path.join(ROOT, "scenes", file), "utf8"));
  if (!scene.script?.rawText) { skipped += 1; continue; }
  const rt = new GraphBoardRuntime(stage, null);
  rt.scene = scene;
  rt.onLoadPage = () => {};
  try {
    rt.executeHandlerIfPresent("OnOpenPage");
    rt.executeHandlerIfPresent("OnTimer", [1]);
    rt.executeHandlerIfPresent("OnLButtonDown", [320, 240]);
    rt.executeHandlerIfPresent("OnKeyDown", [32]);
    rt.clearTimers();
    rt.clearVideoTimers();
    rt.clearGlides();
    if (rt.panoramaTimer) clearInterval(rt.panoramaTimer);
    ran += 1;
  } catch (e) {
    crashed += 1;
    console.log(`CRASH ${file}: ${e.message}`);
  }
}
console.log(`ran=${ran} skipped=${skipped} crashed=${crashed}`);
process.exit(crashed ? 1 : 0);
