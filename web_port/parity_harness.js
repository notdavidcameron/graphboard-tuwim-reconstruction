async function loadParityScenarios() {
  const response = await fetch(`parity_scenarios.json?v=${window.GRAPHBOARD_CACHE_VERSION || "dev"}`);
  if (!response.ok) throw new Error(`parity_scenarios.json: ${response.status}`);
  return response.json();
}

async function runParityScenario(scenario) {
  const runtime = window.graphBoardRuntime;
  if (!runtime) throw new Error("GraphBoard runtime is not available");
  runtime.traces = [];
  for (const step of scenario.steps || []) {
    if (step.action === "activate") {
      runtime.markUserActivated();
    } else if (step.action === "handler") {
      runtime.executeHandler(step.name, step.args || []);
    } else if (step.action === "event") {
      runtime.dispatchComponentEvent(step.component, step.event, Number(step.id), step.extra || []);
    }
  }
  const seen = new Set(runtime.traces.map((item) => item.event));
  return {
    id: scenario.id,
    scene: scenario.scene,
    passed: (scenario.expectEvents || []).every((event) => seen.has(event)),
    missing: (scenario.expectEvents || []).filter((event) => !seen.has(event)),
    traces: runtime.traces,
  };
}

window.GraphBoardParity = {
  load: loadParityScenarios,
  run: runParityScenario,
};
