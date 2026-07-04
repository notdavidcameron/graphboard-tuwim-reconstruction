#!/usr/bin/env python3
"""Export extracted GraphBoard assets into browser scene JSON."""

from __future__ import annotations

import argparse
import json
import re
import struct
from pathlib import Path
from typing import Any


IMAGE_KINDS = {"bmp", "dib", "board_video_gif", "multibitmap_bmp", "multibitmap_gif"}
AUDIO_KINDS = {"WAVE", "riff_wave", "board_video_pcm_wave"}


def safe_id(value: str) -> str:
    value = re.sub(r"[^A-Za-z0-9._-]+", "_", value.strip())
    return value.strip("_") or "scene"


def read_json(path: Path) -> Any:
    return json.loads(path.read_text(encoding="utf-8"))


def write_json(path: Path, payload: Any) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(payload, indent=2, ensure_ascii=False), encoding="utf-8")


def rel_url(path: str | None, web_root: Path, workspace_root: Path) -> str | None:
    if not path:
        return None
    asset = Path(path)
    try:
        rel = asset.resolve().relative_to(workspace_root.resolve())
        return "../" + rel.as_posix()
    except ValueError:
        try:
            rel = asset.resolve().relative_to(web_root.resolve())
            return rel.as_posix()
        except ValueError:
            return asset.as_posix()


def parse_project_order(project_path: Path | None, available_ids: set[str]) -> tuple[list[str], list[str]]:
    warnings: list[str] = []
    if not project_path or not project_path.exists():
        return [], ["START.PRJ not found; scene order is alphabetical."]
    data = project_path.read_bytes()
    names = [
        item.decode("cp1250", "replace").upper().removesuffix(".BDF")
        for item in re.findall(rb"[A-Za-z0-9_]{2,16}\.bdf", data, flags=re.IGNORECASE)
    ]
    ordered: list[str] = []
    for name in names:
        if name in available_ids and name not in ordered:
            ordered.append(name)
    if not ordered:
        warnings.append("START.PRJ was found but no BDF names matched extracted scene folders.")
    missing = sorted(available_ids.difference(ordered))
    ordered.extend(missing)
    if missing:
        warnings.append(f"{len(missing)} scene(s) were not listed in START.PRJ and were appended alphabetically.")
    return ordered, warnings


HANDLER_PATTERN = re.compile(
    r"(?m)^\s*(?:(?:void|int|CString|float|double|char|long)\s+)?"
    r"([A-Za-z_][A-Za-z0-9_.]*)\s*\(([^;{}]*)\)\s*\{"
)


def parse_script(script_path: Path | None, web_root: Path, workspace_root: Path) -> dict[str, Any]:
    if not script_path or not script_path.exists():
        return {"path": None, "url": None, "handlers": [], "rawText": ""}
    text = script_path.read_text(encoding="utf-8", errors="replace")
    handlers = []
    for match in HANDLER_PATTERN.finditer(text):
        handlers.append({"name": match.group(1), "signature": f"{match.group(1)}({match.group(2).strip()})"})
    return {
        "path": str(script_path),
        "url": rel_url(str(script_path), web_root, workspace_root),
        "handlers": handlers,
        "rawText": text,
    }


def script_geometry_hints(script_text: str) -> dict[str, dict[int, dict[str, int]]]:
    hints: dict[str, dict[int, dict[str, int]]] = {
        "MultiBitmap": {},
        "Sprite_Holder": {},
        "Transparent_Video_Holder": {},
    }

    for match in re.finditer(
        r"\bMultiBitmap\.ShowBitmap\s*\(\s*(\d+)\s*,\s*(-?\d+)\s*,\s*(-?\d+)\s*,\s*(-?\d+)\s*\)",
        script_text,
    ):
        item_id, x, y, z = [int(part) for part in match.groups()]
        hints["MultiBitmap"].setdefault(item_id, {"x": x, "y": y, "z": z})

    for holder in ("Sprite_Holder", "Group.Sprite_Holder"):
        for match in re.finditer(
            rf"\b{re.escape(holder)}\.GotoXY\s*\(\s*(\d+)\s*,\s*(-?\d+)\s*,\s*(-?\d+)\s*\)",
            script_text,
        ):
            item_id, x, y = [int(part) for part in match.groups()]
            hints["Sprite_Holder"].setdefault(item_id, {"x": x, "y": y, "z": 20})

    for match in re.finditer(r"\bTransparent_Video_Holder\.Play\s*\(\s*(\d+)\s*,", script_text):
        item_id = int(match.group(1))
        hints["Transparent_Video_Holder"].setdefault(item_id, {"x": 0, "y": 0, "z": 10})

    return hints


def asset_id_from_path(path: str) -> int | None:
    name = Path(path).name
    match = re.search(r"_(\d{3})_", name)
    if not match:
        return None
    return int(match.group(1))


def normalized_board_video_id(path: str) -> int | None:
    raw = asset_id_from_path(path)
    if raw is None:
        return None
    return raw // 2


def read_u32(data: bytes, offset: int) -> int:
    return struct.unpack_from("<I", data, offset)[0]


def read_i32(data: bytes, offset: int) -> int:
    return struct.unpack_from("<i", data, offset)[0]


def board_video_still_size(width: int, height: int) -> int:
    stride = ((width * 8 + 0x1F) >> 3) & 0x1FFFFFFC
    return stride * height


def parse_transparent_video_entries(meta_component: dict[str, Any], bdf_data: bytes | None) -> dict[int, dict[str, Any]]:
    """Recover per-video stage geometry from TransparentVideoHolder's serialized entries."""
    if meta_component.get("display_name") != "Transparent_Video_Holder" or not bdf_data:
        return {}

    private_start = int(meta_component.get("private_offset") or 0)
    private_size = int(meta_component.get("private_size") or 0)
    private_end = min(len(bdf_data), private_start + private_size)
    if private_start + 8 > private_end:
        return {}

    try:
        version = read_u32(bdf_data, private_start)
        declared_count = read_u32(bdf_data, private_start + 4)
    except struct.error:
        return {}
    if version not in (1, 2, 3) or declared_count <= 0 or declared_count > 1000:
        return {}

    entries: dict[int, dict[str, Any]] = {}
    cursor = private_start + 8
    for index in range(declared_count):
        if cursor + 4 > private_end:
            break
        try:
            video_size = read_u32(bdf_data, cursor)
        except struct.error:
            break
        stream_offset = cursor + 4
        entry_offset = stream_offset + video_size
        if (
            video_size <= 0
            or stream_offset + 0x4E8 > private_end
            or entry_offset + 0x568 > private_end
            or bdf_data[stream_offset : stream_offset + 16] != b"Board Video File"
            or bdf_data[entry_offset : entry_offset + 16] != b"Board Video File"
        ):
            break

        try:
            width = read_u32(bdf_data, entry_offset + 0x80)
            height = read_u32(bdf_data, entry_offset + 0x84)
            x = read_i32(bdf_data, entry_offset + 0x54C)
            y = read_i32(bdf_data, entry_offset + 0x550)
            z = read_i32(bdf_data, entry_offset + 0x554)
            original_x = read_i32(bdf_data, entry_offset + 0x508)
            original_y = read_i32(bdf_data, entry_offset + 0x50C)
            original_z = read_i32(bdf_data, entry_offset + 0x510)
        except struct.error:
            break

        if not (0 < width <= 2000 and 0 < height <= 2000):
            break
        if not (-4000 <= x <= 4000 and -4000 <= y <= 4000 and -10000 <= z <= 10000):
            x, y, z = original_x, original_y, original_z

        entries[index] = {
            "rect": {"x": x, "y": y, "width": width, "height": height},
            "z": z,
            "geometryConfidence": "serialized_entry",
            "entryOffset": entry_offset,
            "streamOffset": stream_offset,
            "videoSize": video_size,
            "originalRect": {"x": original_x, "y": original_y, "width": width, "height": height},
            "originalZ": original_z,
        }
        cursor = entry_offset + 0x568 + board_video_still_size(width, height)
    return entries


def image_asset(item: dict[str, Any], web_root: Path, workspace_root: Path) -> dict[str, Any]:
    path = item.get("path") or item.get("video_output")
    return {
        "kind": item.get("kind"),
        "path": path,
        "url": rel_url(path, web_root, workspace_root),
        "width": item.get("width") or item.get("video_width"),
        "height": item.get("height") or item.get("video_height"),
        "frameCount": item.get("frame_count"),
        "offset": item.get("offset"),
        "name": item.get("name"),
        "id": asset_id_from_path(path or ""),
    }


def audio_asset(item: dict[str, Any], web_root: Path, workspace_root: Path) -> dict[str, Any]:
    path = item.get("path") or item.get("audio_output")
    return {
        "kind": item.get("kind"),
        "path": path,
        "url": rel_url(path, web_root, workspace_root),
        "durationSeconds": item.get("duration_seconds"),
        "sampleRate": item.get("sample_rate"),
        "channels": item.get("channels"),
        "offset": item.get("offset"),
        "id": asset_id_from_path(path or ""),
    }


def pick_rect(component_type: str, asset: dict[str, Any] | None, hints: dict[str, dict[int, dict[str, int]]]) -> tuple[dict[str, int], str, int]:
    width = int(asset.get("width") or 120) if asset else 120
    height = int(asset.get("height") or 80) if asset else 80
    z = 0
    x = 0
    y = 0
    confidence = "unknown"

    if component_type == "MultiBitmap" and asset:
        item_id = asset.get("id")
        if isinstance(item_id, int) and item_id in hints["MultiBitmap"]:
            hint = hints["MultiBitmap"][item_id]
            x, y, z = hint["x"], hint["y"], hint["z"]
            confidence = "script_explicit"
        elif width == 640 and height == 480:
            confidence = "full_page_size"
    elif component_type == "Transparent_Video_Holder" and asset:
        item_id = normalized_board_video_id(asset.get("path") or "")
        if isinstance(item_id, int) and item_id in hints["Transparent_Video_Holder"] and width == 640 and height == 480:
            confidence = "script_reference_full_page"
            z = hints["Transparent_Video_Holder"][item_id]["z"]
        elif width == 640 and height == 480:
            confidence = "full_page_size"
            z = 10
    elif asset and width == 640 and height == 480:
        confidence = "full_page_size"

    return {"x": x, "y": y, "width": width, "height": height}, confidence, z


def build_component(
    meta_component: dict[str, Any],
    web_root: Path,
    workspace_root: Path,
    hints: dict[str, dict[int, dict[str, int]]],
    bdf_data: bytes | None = None,
) -> dict[str, Any]:
    images = []
    audios = []
    for item in meta_component.get("embedded", []):
        kind = item.get("kind")
        if kind in IMAGE_KINDS:
            images.append(image_asset(item, web_root, workspace_root))
        elif kind in AUDIO_KINDS:
            audios.append(audio_asset(item, web_root, workspace_root))
    tvh_entries = parse_transparent_video_entries(meta_component, bdf_data)
    if tvh_entries:
        for asset in images:
            item_id = normalized_board_video_id(asset.get("path") or "")
            if isinstance(item_id, int) and item_id in tvh_entries:
                entry = tvh_entries[item_id]
                asset["rect"] = entry["rect"]
                asset["z"] = entry["z"]
                asset["geometryConfidence"] = entry["geometryConfidence"]
                asset["entryOffset"] = entry["entryOffset"]
                asset["streamOffset"] = entry["streamOffset"]
                asset["originalRect"] = entry["originalRect"]
                asset["originalZ"] = entry["originalZ"]
    primary = images[0] if images else None
    rect, confidence, z = pick_rect(meta_component.get("display_name", ""), primary, hints)
    known_rects = [asset["rect"] for asset in images if asset.get("geometryConfidence") == "serialized_entry"]
    if known_rects:
        min_x = min(rect["x"] for rect in known_rects)
        min_y = min(rect["y"] for rect in known_rects)
        max_x = max(rect["x"] + rect["width"] for rect in known_rects)
        max_y = max(rect["y"] + rect["height"] for rect in known_rects)
        rect = {"x": min_x, "y": min_y, "width": max_x - min_x, "height": max_y - min_y}
        confidence = "serialized_entries"
        z = min(int(asset.get("z") or 0) for asset in images if asset.get("geometryConfidence") == "serialized_entry")
    if meta_component.get("display_name") == "Sound_Holder":
        confidence = "not_visual"
        rect = {"x": 0, "y": 0, "width": 0, "height": 0}
    return {
        "id": f"{meta_component.get('index'):02d}_{safe_id(meta_component.get('display_name', 'component'))}",
        "type": meta_component.get("display_name"),
        "index": meta_component.get("index"),
        "z": z if confidence != "unknown" else meta_component.get("index", 0),
        "visible": bool(primary and confidence != "not_visual"),
        "rect": rect,
        "geometryConfidence": confidence,
        "sourceOffset": meta_component.get("offset"),
        "privateOffset": meta_component.get("private_offset"),
        "privateSize": meta_component.get("private_size"),
        "primaryAssetUrl": primary.get("url") if primary else None,
        "assets": images,
        "audio": audios,
    }


def build_scene(meta_path: Path, web_root: Path, workspace_root: Path) -> dict[str, Any]:
    meta = read_json(meta_path)
    page_dir = meta_path.parent
    script_path = page_dir / "script.txt"
    script = parse_script(script_path if script_path.exists() else None, web_root, workspace_root)
    hints = script_geometry_hints(script["rawText"])
    source_path = Path(meta.get("source") or "")
    bdf_data = source_path.read_bytes() if source_path.exists() else None
    components = [
        build_component(component, web_root, workspace_root, hints, bdf_data) for component in meta.get("components", [])
    ]
    background_bmp = page_dir / "background.bmp"
    palette = page_dir / "background_palette.bin"

    warnings = []
    if not background_bmp.exists() and meta.get("dib_size", 0):
        warnings.append("DIB exists in metadata, but converted background.bmp was not found.")
    if not background_bmp.exists() and not components:
        warnings.append("No background image and no components found.")
    unknown = sum(1 for component in components if component["geometryConfidence"] == "unknown")
    if unknown:
        warnings.append(f"{unknown} component(s) have unknown geometry and default to 0,0.")
    recovered_tvh = sum(
        1
        for component in components
        for asset in component.get("assets", [])
        if asset.get("geometryConfidence") == "serialized_entry"
    )
    if recovered_tvh:
        warnings.append(f"Recovered {recovered_tvh} TransparentVideoHolder asset rect(s) from serialized entries.")

    return {
        "id": meta_path.parent.name,
        "title": meta_path.parent.name,
        "sourceBdf": meta.get("source"),
        "pageRect": meta.get("page_rect", [0, 0, 640, 480]),
        "background": {
            "colorIndex": meta.get("background_color_index"),
            "flag": meta.get("background_flag"),
            "bmpPath": str(background_bmp) if background_bmp.exists() else None,
            "bmpUrl": rel_url(str(background_bmp), web_root, workspace_root) if background_bmp.exists() else None,
            "palettePath": str(palette) if palette.exists() else None,
            "paletteUrl": rel_url(str(palette), web_root, workspace_root) if palette.exists() else None,
        },
        "components": components,
        "script": script,
        "warnings": warnings,
    }


def validate_scene(scene: dict[str, Any], workspace_root: Path) -> list[str]:
    errors = []
    urls = []
    bg = scene.get("background", {})
    if bg.get("bmpUrl"):
        urls.append(bg["bmpUrl"])
    if scene.get("script", {}).get("url"):
        urls.append(scene["script"]["url"])
    for component in scene.get("components", []):
        for asset in component.get("assets", []):
            if asset.get("url"):
                urls.append(asset["url"])
        for asset in component.get("audio", []):
            if asset.get("url"):
                urls.append(asset["url"])
    for url in urls:
        if not url.startswith("../"):
            continue
        local = workspace_root / url.removeprefix("../")
        if not local.exists():
            errors.append(f"missing asset: {url}")
    return errors


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--extracted", type=Path, default=Path("extracted_assets"))
    parser.add_argument("--output", type=Path, default=Path("web_port"))
    parser.add_argument("--project", type=Path)
    args = parser.parse_args()

    workspace_root = Path.cwd()
    web_root = args.output
    scenes_dir = web_root / "scenes"
    scenes_dir.mkdir(parents=True, exist_ok=True)

    metadata_paths = sorted((args.extracted / "bdf").glob("*/metadata.json"))
    available = {path.parent.name.upper(): path for path in metadata_paths}
    order, order_warnings = parse_project_order(args.project, set(available))
    if not order:
        order = sorted(available)

    index_entries = []
    validation = {
        "missingAssets": [],
        "componentsWithoutGeometry": 0,
        "scenesWithoutBackgrounds": 0,
        "scriptsFound": 0,
        "scriptsMissing": 0,
        "totalPages": 0,
        "totalComponents": 0,
        "totalAssets": 0,
        "warnings": order_warnings,
    }

    for scene_id in order:
        meta_path = available.get(scene_id)
        if not meta_path:
            continue
        scene = build_scene(meta_path, web_root, workspace_root)
        write_json(scenes_dir / f"{scene['id']}.json", scene)
        component_count = len(scene["components"])
        asset_count = sum(len(component["assets"]) + len(component["audio"]) for component in scene["components"])
        missing = validate_scene(scene, workspace_root)
        validation["missingAssets"].extend({"scene": scene["id"], "error": item} for item in missing)
        validation["componentsWithoutGeometry"] += sum(
            1 for component in scene["components"] if component["geometryConfidence"] == "unknown"
        )
        validation["scenesWithoutBackgrounds"] += 0 if scene["background"].get("bmpUrl") else 1
        validation["scriptsFound"] += 1 if scene["script"].get("url") else 0
        validation["scriptsMissing"] += 0 if scene["script"].get("url") else 1
        validation["totalPages"] += 1
        validation["totalComponents"] += component_count
        validation["totalAssets"] += asset_count
        index_entries.append(
            {
                "id": scene["id"],
                "title": scene["title"],
                "url": f"scenes/{scene['id']}.json",
                "sourceBdf": scene["sourceBdf"],
                "componentCount": component_count,
                "assetCount": asset_count,
                "warningCount": len(scene["warnings"]),
                "hasBackground": bool(scene["background"].get("bmpUrl")),
                "hasScript": bool(scene["script"].get("url")),
            }
        )

    index = {
        "title": "GraphBoard HTML Port V1",
        "generatedFrom": str(args.extracted),
        "projectOrderSource": str(args.project) if args.project else None,
        "orderWarnings": order_warnings,
        "scenes": index_entries,
        "validation": validation,
    }
    write_json(scenes_dir / "index.json", index)
    write_json(web_root / "validation.json", validation)
    print(json.dumps(validation, indent=2, ensure_ascii=False))
    return 1 if validation["missingAssets"] else 0


if __name__ == "__main__":
    raise SystemExit(main())
