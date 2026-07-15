#!/usr/bin/env python3
"""Export extracted GraphBoard assets into browser scene JSON."""

from __future__ import annotations

import argparse
import json
import re
import struct
from pathlib import Path
from typing import Any


IMAGE_KINDS = {
    "bmp",
    "dib",
    "board_video_gif",
    "multibitmap_bmp",
    "bitmap_holder_bmp",
    "sprite_holder_png",
    "panorama_bmp",
    "panorama_holder_bmp",
}
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


def parse_project_file(project_path: Path | None) -> dict[str, Any] | None:
    """Parse START.PRJ exactly per GraphBrdDoc_SerializeProjectState (Tuwim.exe:00406020).

    Layout (little-endian, MFC CArchive), verified against START.PRJ:
        u32 version                     # 1
        CString startupPage             # e.g. "intro.bdf"
        CString currentPageOrGroupState # usually empty
        u32 audioPresetIndex            # audioManager(+0xbc)->[+0x20]
        u32 pageCount;  CString pageNames[pageCount]
        u32 groupCount; CString groupNames[groupCount]
        CString signature               # "Julian Tuwim", each byte +0x21 on save
        u32 scriptVersion               # global script editor (document field +0xa0)
        CString globalScript            # project-wide global-variable/handler script

    The trailing (scriptVersion, globalScript) pair is written by the group/global
    script editor's Serialize (GraphBrdScriptEditor_SerializeText, Tuwim.exe:004230a0),
    the same routine the page serializer uses for per-page script text. It is NOT
    audio-manager state; the audio manager only contributes audioPresetIndex.

    Returns a dict of recovered fields, or None if the bytes do not match the
    expected structure (caller falls back to a permissive scan).
    """
    if not project_path or not project_path.exists():
        return None
    data = project_path.read_bytes()
    pos = 0

    def u32() -> int:
        nonlocal pos
        value = struct.unpack_from("<I", data, pos)[0]
        pos += 4
        return value

    def cstring_bytes() -> bytes:
        # MFC CArchive string length prefix: u8, or 0xff+u16, or 0xff 0xffff+u32.
        nonlocal pos
        length = data[pos]
        pos += 1
        if length == 0xFF:
            length = struct.unpack_from("<H", data, pos)[0]
            pos += 2
            if length == 0xFFFF:
                length = struct.unpack_from("<I", data, pos)[0]
                pos += 4
        raw = data[pos : pos + length]
        pos += length
        return raw

    try:
        version = u32()
        startup_page = cstring_bytes().decode("cp1250", "replace")
        current_state = cstring_bytes().decode("cp1250", "replace")
        audio_preset_index = u32()
        page_count = u32()
        if page_count > 4096:
            return None
        page_names = [cstring_bytes().decode("cp1250", "replace") for _ in range(page_count)]
        group_count = u32()
        if group_count > 4096:
            return None
        group_names = [cstring_bytes().decode("cp1250", "replace") for _ in range(group_count)]
        signature = bytes((b - 0x21) & 0xFF for b in cstring_bytes()).decode("latin1", "replace")
        script_version = u32()
        global_script = cstring_bytes().decode("cp1250", "replace")
    except (struct.error, IndexError):
        return None

    return {
        "version": version,
        "startupPage": startup_page,
        "currentState": current_state,
        "audioPresetIndex": audio_preset_index,
        "pageNames": page_names,
        "groupNames": group_names,
        "signature": signature,
        "signatureValid": signature == "Julian Tuwim",
        "scriptVersion": script_version,
        "globalScript": global_script,
        "trailingBytes": len(data) - pos,
    }


def parse_project_order(project_path: Path | None, available_ids: set[str]) -> tuple[list[str], list[str]]:
    warnings: list[str] = []
    if not project_path or not project_path.exists():
        return [], ["START.PRJ not found; scene order is alphabetical."]
    project = parse_project_file(project_path)
    if project is not None:
        names = [name.upper().removesuffix(".BDF") for name in project["pageNames"]]
    else:
        warnings.append("START.PRJ did not match the expected project layout; used a permissive name scan.")
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
    r"(?im)^\s*(?:(?:void|bool|int|CString|float|double|char|long|short|UINT|DWORD|BOOL|VOID)\s+)?"
    r"(?!(?:if|else|while|switch|for|do)\b)"
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


def read_archive_string(data: bytes, offset: int, end: int) -> tuple[str, int]:
    """Read an MFC CArchive CString and return (decoded_text, new_offset)."""
    if offset >= end:
        raise struct.error("CString length is outside the private block")
    length = data[offset]
    offset += 1
    if length == 0xFF:
        if offset + 2 > end:
            raise struct.error("CString u16 length is outside the private block")
        length = struct.unpack_from("<H", data, offset)[0]
        offset += 2
        if length == 0xFFFF:
            if offset + 4 > end:
                raise struct.error("CString u32 length is outside the private block")
            length = read_u32(data, offset)
            offset += 4
    if offset + length > end:
        raise struct.error("CString payload is outside the private block")
    raw = data[offset : offset + length]
    return raw.decode("cp1250", "replace"), offset + length


def runtime_component_type(component_type: str | None) -> str:
    value = str(component_type or "")
    return value.removeprefix("Group.")


def parse_hotspot_entries(meta_component: dict[str, Any], source_data: bytes | None) -> list[dict[str, Any]]:
    """Recover HotSpotHolder rectangles from the DLL serializer layout."""
    if runtime_component_type(meta_component.get("display_name")) != "HotSpot_Holder" or not source_data:
        return []
    private_start = int(meta_component.get("private_offset") or 0)
    private_size = int(meta_component.get("private_size") or 0)
    private_end = min(len(source_data), private_start + private_size)
    if private_start + 19 > private_end:
        return []
    try:
        version = read_u32(source_data, private_start)
        flag0 = source_data[private_start + 4]
        flag1 = source_data[private_start + 5]
        flag2 = source_data[private_start + 6]
        count = read_u32(source_data, private_start + 7)
    except struct.error:
        return []
    if version != 0 or count <= 0 or count > 100 or private_start + 11 + count * 100 + 8 > private_end:
        return []

    entries: list[dict[str, Any]] = []
    cursor = private_start + 11
    for index in range(count):
        try:
            left = read_i32(source_data, cursor)
            top = read_i32(source_data, cursor + 4)
            right = read_i32(source_data, cursor + 8)
            bottom = read_i32(source_data, cursor + 12)
            hotspot_id = read_i32(source_data, cursor + 0x18)
            layer = read_i32(source_data, cursor + 0x1C)
            enabled = read_i32(source_data, cursor + 0x20)
            name, next_cursor = read_archive_string(source_data, cursor + 100, private_end)
        except struct.error:
            return []
        width = max(1, right - left)
        height = max(1, bottom - top)
        if -5000 <= left <= 5000 and -5000 <= top <= 5000 and 0 < width <= 5000 and 0 < height <= 5000:
            entries.append(
                {
                    # The holder addresses hotspots by this serialized id, not
                    # by their position in the record array.
                    "id": hotspot_id,
                    "rect": {"x": left, "y": top, "width": width, "height": height},
                    "name": name,
                    "layer": layer,
                    "enabled": enabled != 0,
                    "flags": [flag0, flag1, flag2],
                    "sourceOffset": cursor,
                    "geometryConfidence": "serialized_entry",
                }
            )
        cursor = next_cursor
    if cursor + 8 <= private_end:
        try:
            active_index = read_u32(source_data, cursor)
            aux_state = read_u32(source_data, cursor + 4)
        except struct.error:
            active_index = None
            aux_state = None
        for entry in entries:
            entry["activeIndex"] = active_index
            entry["auxState"] = aux_state
    return entries


def plausible_rect(left: int, top: int, right: int, bottom: int, limit: int = 5000) -> dict[str, int] | None:
    width = right - left
    height = bottom - top
    if -limit <= left <= limit and -limit <= top <= limit and 0 < width <= limit and 0 < height <= limit:
        return {"x": left, "y": top, "width": width, "height": height}
    return None


def parse_bitmap_rect(meta_component: dict[str, Any], source_data: bytes | None) -> dict[str, Any] | None:
    """Recover BitmapHolder's first visible rectangle.

    The observed BitmapHolder private block starts with version/count/bitmap-size,
    followed by a per-item blob. The stable stage rectangle is left/top/right/bottom
    at +0x14..+0x20 in the private block.
    """
    if runtime_component_type(meta_component.get("display_name")) != "Bitmap_Holder" or not source_data:
        return None
    private_start = int(meta_component.get("private_offset") or 0)
    private_size = int(meta_component.get("private_size") or 0)
    private_end = min(len(source_data), private_start + private_size)
    if private_start + 36 > private_end:
        return None
    try:
        version = read_u32(source_data, private_start)
        count = read_u32(source_data, private_start + 4)
        left = read_i32(source_data, private_start + 0x14)
        top = read_i32(source_data, private_start + 0x18)
        right = read_i32(source_data, private_start + 0x1C)
        bottom = read_i32(source_data, private_start + 0x20)
    except struct.error:
        return None
    rect = plausible_rect(left, top, right, bottom)
    if version != 1 or count <= 0 or count > 200 or not rect:
        return None
    return {"rect": rect, "sourceOffset": private_start + 0x14, "geometryConfidence": "serialized_entry"}


def parse_text_rect(meta_component: dict[str, Any], source_data: bytes | None) -> dict[str, Any] | None:
    """Recover TextHolder's first text-region rectangle.

    Corpus samples begin with u32 version, u32 entry count, then an entry whose
    x/y/width/height are stored at +0x0c..+0x18. A duplicate right/bottom rectangle
    follows later in the same record, but these leading dimensions are enough for a
    conservative visual/runtime placement.
    """
    if runtime_component_type(meta_component.get("display_name")) != "Text_Holder" or not source_data:
        return None
    private_start = int(meta_component.get("private_offset") or 0)
    private_size = int(meta_component.get("private_size") or 0)
    private_end = min(len(source_data), private_start + private_size)
    if private_start + 28 > private_end:
        return None
    try:
        version = read_u32(source_data, private_start)
        count = read_u32(source_data, private_start + 4)
        x = read_i32(source_data, private_start + 0x0C)
        y = read_i32(source_data, private_start + 0x10)
        width = read_i32(source_data, private_start + 0x14)
        height = read_i32(source_data, private_start + 0x18)
    except struct.error:
        return None
    rect = plausible_rect(x, y, x + width, y + height)
    if version not in (1, 2) or count <= 0 or count > 200 or not rect:
        return None
    return {"rect": rect, "sourceOffset": private_start + 0x0C, "geometryConfidence": "serialized_entry"}


def parse_video_rect(meta_component: dict[str, Any], source_data: bytes | None) -> dict[str, Any] | None:
    if runtime_component_type(meta_component.get("display_name")) != "Video_Holder" or not source_data:
        return None
    private_start = int(meta_component.get("private_offset") or 0)
    private_size = int(meta_component.get("private_size") or 0)
    private_end = min(len(source_data), private_start + private_size)
    if private_start + 32 > private_end:
        return None
    try:
        version = read_u32(source_data, private_start)
        width = read_i32(source_data, private_start + 0x18)
        height = read_i32(source_data, private_start + 0x1C)
    except struct.error:
        return None
    if version == 1 and 0 < width <= 2000 and 0 < height <= 2000:
        return {
            "rect": {"x": 0, "y": 0, "width": width, "height": height},
            "sourceOffset": private_start + 0x18,
            "geometryConfidence": "declared_size",
        }
    return None


def parse_sprite_item_count(meta_component: dict[str, Any], source_data: bytes | None) -> int | None:
    if runtime_component_type(meta_component.get("display_name")) != "Sprite_Holder" or not source_data:
        return None
    private_start = int(meta_component.get("private_offset") or 0)
    private_size = int(meta_component.get("private_size") or 0)
    if private_start + 12 > min(len(source_data), private_start + private_size):
        return None
    try:
        version = read_u32(source_data, private_start)
        definition_count = read_u32(source_data, private_start + 4)
        instance_count = read_u32(source_data, private_start + 8)
    except struct.error:
        return None
    if version == 1 and 0 <= definition_count <= 200 and 0 <= instance_count <= 200:
        return instance_count
    return None


def board_video_still_size(width: int, height: int) -> int:
    stride = ((width * 8 + 0x1F) >> 3) & 0x1FFFFFFC
    return stride * height


def parse_transparent_video_entries(meta_component: dict[str, Any], bdf_data: bytes | None) -> dict[int, dict[str, Any]]:
    """Recover per-video stage geometry from TransparentVideoHolder's serialized entries."""
    if runtime_component_type(meta_component.get("display_name")) != "Transparent_Video_Holder" or not bdf_data:
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
    render_path = path
    if item.get("kind") == "bitmap_holder_bmp" and item.get("png_path"):
        render_path = item.get("png_path")
    still_path = item.get("still_path") or item.get("still_output")
    asset = {
        "kind": item.get("kind"),
        "path": path,
        "url": rel_url(render_path, web_root, workspace_root),
        "renderPath": render_path,
        "stillPath": still_path,
        "stillUrl": rel_url(still_path, web_root, workspace_root),
        "width": item.get("width") or item.get("video_width"),
        "height": item.get("height") or item.get("video_height"),
        "frameCount": item.get("frame_count"),
        "offset": item.get("offset"),
        "name": item.get("name"),
        "id": item.get("id") if item.get("id") is not None else asset_id_from_path(path or ""),
    }
    if item.get("definition_id") is not None:
        asset["definitionId"] = item.get("definition_id")
    if item.get("phase_outputs"):
        asset["phaseAssets"] = [
            {
                "phase": phase.get("phase"),
                "path": phase.get("path"),
                "url": rel_url(phase.get("path"), web_root, workspace_root),
            }
            for phase in item.get("phase_outputs", [])
        ]
        asset["phaseCount"] = item.get("phase_count") or len(asset["phaseAssets"])
    if item.get("initially_visible") is not None:
        asset["initiallyVisible"] = bool(item.get("initially_visible"))
    if item.get("rect"):
        asset["rect"] = item["rect"]
        asset["geometryConfidence"] = item.get("geometry_confidence") or "serialized_entry"
    return asset


def expand_sprite_instances(meta_component: dict[str, Any], definition_assets: list[dict[str, Any]]) -> list[dict[str, Any]]:
    if runtime_component_type(meta_component.get("display_name")) != "Sprite_Holder":
        return definition_assets

    holder = meta_component.get("sprite_holder") or {}
    instances = holder.get("instances") or []
    if not instances:
        return definition_assets

    by_definition: dict[int, dict[str, Any]] = {}
    for asset in definition_assets:
        definition_id = asset.get("definitionId")
        if definition_id is None:
            definition_id = asset.get("id")
        if isinstance(definition_id, int):
            by_definition[definition_id] = asset

    expanded = []
    for instance in instances:
        definition_id = instance.get("definition_id")
        if not isinstance(definition_id, int):
            continue
        source = by_definition.get(definition_id)
        if not source:
            continue
        try:
            x = int(instance.get("x"))
            y = int(instance.get("y"))
        except (TypeError, ValueError):
            continue
        width = int(source.get("width") or 0)
        height = int(source.get("height") or 0)
        if width <= 0 or height <= 0:
            continue

        sprite = dict(source)
        sprite["id"] = int(instance.get("index") or 0)
        sprite["definitionId"] = definition_id
        sprite["instanceOffset"] = instance.get("record_offset")
        sprite["rect"] = {"x": x, "y": y, "width": width, "height": height}
        sprite["geometryConfidence"] = "serialized_instance"
        phase_count = int(sprite.get("phaseCount") or 1)
        initial_phase = int(instance.get("phase") or 0)
        sprite["initialPhase"] = initial_phase if 0 <= initial_phase < phase_count else 0
        sprite["initiallyVisible"] = bool(instance.get("visible"))
        sprite["draggable"] = bool(instance.get("drag_enabled"))
        if isinstance(instance.get("z"), int):
            sprite["z"] = instance["z"]
        expanded.append(sprite)

    return expanded or definition_assets


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


PUZZLE_FALLBACK_PREFIXES = [
    ("ABECADP", "ABECADLO"),
    ("CUDAP", "CUDA"),
    ("DYZIOP", "DYZIO"),
    ("FIGIELP", "FIGIELEK"),
    ("GRZESP", "GRZESIU"),
    ("KOT", "KOTEK"),
    ("KRAWIEP", "KRAWIEC"),
    ("LOK", "LOKOMOT"),
    ("MALUSP", "MALUSKIE"),
    ("MICHALP", "MICH0"),
    ("MROZP", "MROZ"),
    ("MURARZP", "MURARZ"),
    ("MURZYNP", "MURZYNEK"),
    ("OKUL", "OKULARY"),
    ("PIEKARP", "PIEKARZ"),
    ("PLOTKIP", "PLOTKI"),
    ("PSTR", "PSTRYK"),
    ("RACHUNP", "RACHUNEK"),
    ("RADIOP", "RADIO"),
    ("RYCE", "RYCERZ"),
    ("RZECZ", "RZECZKA"),
    ("SLON", "SLON"),
    ("SLOW", "SLOWIK"),
    ("SZEWCP", "SZEWC"),
    ("TANIE", "TANIEC"),
    ("TRALALP", "TRALALA"),
    ("WIES", "WIES"),
    ("WSZYSCP", "WSZYSCY"),
    ("ZOSIAP", "ZOSIA"),
]


def bmp_dimensions(path: Path) -> tuple[int | None, int | None]:
    if not path.exists():
        return None, None
    data = path.read_bytes()[:26]
    if len(data) < 26 or data[:2] != b"BM":
        return None, None
    return struct.unpack_from("<i", data, 18)[0], abs(struct.unpack_from("<i", data, 22)[0])


def largest_visual_asset(scene_dir: Path) -> tuple[Path, int | None, int | None] | None:
    background = scene_dir / "background.bmp"
    if background.exists():
        width, height = bmp_dimensions(background)
        return background, width, height
    metadata = scene_dir / "metadata.json"
    if not metadata.exists():
        return None
    meta = read_json(metadata)
    candidates = []
    for output in meta.get("outputs", []):
        if output.get("kind") not in IMAGE_KINDS:
            continue
        path = Path(output.get("path") or "")
        if not path.exists():
            continue
        width = output.get("width")
        height = output.get("height")
        area = int(width or 0) * int(height or 0)
        candidates.append((area, path, width, height))
    if not candidates:
        return None
    _area, path, width, height = max(candidates, key=lambda item: item[0])
    return path, width, height


def puzzle_fallback_background(scene_id: str, bdf_root: Path) -> tuple[Path, int | None, int | None] | None:
    candidates = []
    upper_id = scene_id.upper()
    for prefix, target in PUZZLE_FALLBACK_PREFIXES:
        if upper_id.startswith(prefix):
            candidates.append(target)
    candidates.append(re.sub(r"P[12]$", "", upper_id))
    for candidate in candidates:
        visual = largest_visual_asset(bdf_root / candidate)
        if visual:
            return visual
    sibling_prefix = re.sub(r"\d?P[12]$", "", upper_id)
    sibling_candidates = sorted(path for path in bdf_root.glob(f"{sibling_prefix}*") if path.is_dir())
    for sibling in sibling_candidates:
        if re.search(r"P[12]$", sibling.name, re.IGNORECASE):
            continue
        visual = largest_visual_asset(sibling)
        if visual:
            return visual
    return None


def attach_puzzle_fallbacks(
    components: list[dict[str, Any]],
    scene_id: str,
    bdf_root: Path,
    web_root: Path,
    workspace_root: Path,
) -> None:
    fallback = puzzle_fallback_background(scene_id, bdf_root)
    if not fallback:
        return
    fallback_path, width, height = fallback
    for component in components:
        if component.get("type") != "Puzzle" or component.get("assets"):
            continue
        component["assets"].append(
            {
                "kind": "puzzle_fallback_background",
                "path": str(fallback_path),
                "url": rel_url(str(fallback_path), web_root, workspace_root),
                "width": width,
                "height": height,
                "frameCount": None,
                "offset": None,
                "name": fallback_path.parent.name,
                "id": 0,
                "geometryConfidence": "fallback",
            }
        )
        component["primaryAssetUrl"] = component["assets"][0]["url"]
        component["geometryConfidence"] = "fallback"
        component["rect"] = {"x": 0, "y": 0, "width": 580, "height": 480}


def promote_full_page_bitmap_background(components: list[dict[str, Any]], has_background_bmp: bool) -> bool:
    if has_background_bmp:
        return False
    for component in components:
        if component.get("type") != "Bitmap_Holder":
            continue
        for asset in component.get("assets") or []:
            rect = asset.get("rect") or component.get("rect") or {}
            width = int(rect.get("width") or asset.get("width") or 0)
            height = int(rect.get("height") or asset.get("height") or 0)
            x = int(rect.get("x") or 0)
            y = int(rect.get("y") or 0)
            if x <= 0 and y <= 0 and width >= 600 and height >= 440:
                asset["initiallyVisible"] = True
                asset["z"] = min(int(asset.get("z") or component.get("z") or 0), -1)
                component["visible"] = True
                component["z"] = min(int(component.get("z") or 0), -1)
                component["primaryAssetUrl"] = asset.get("url")
                return True
    return False


def pick_rect(component_type: str, asset: dict[str, Any] | None, hints: dict[str, dict[int, dict[str, int]]]) -> tuple[dict[str, int], str, int]:
    component_type = runtime_component_type(component_type)
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
    source_data: bytes | None = None,
    namespace: str = "Page",
) -> dict[str, Any]:
    images = []
    audios = []
    component_type = runtime_component_type(meta_component.get("display_name"))
    for item in meta_component.get("embedded", []):
        kind = item.get("kind")
        if kind in IMAGE_KINDS:
            images.append(image_asset(item, web_root, workspace_root))
        elif kind in AUDIO_KINDS:
            audios.append(audio_asset(item, web_root, workspace_root))
    if component_type == "MultiBitmap":
        for asset in images:
            asset["initiallyVisible"] = False
        if len(images) == 1 and not hints["MultiBitmap"]:
            images[0]["initiallyVisible"] = True
    tvh_entries = parse_transparent_video_entries(meta_component, source_data)
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
    images = expand_sprite_instances(meta_component, images)
    primary = images[0] if images else None
    rect, confidence, z = pick_rect(component_type, primary, hints)
    geometry_source_offset = None
    known_rects = [
        asset["rect"]
        for asset in images
        if asset.get("rect") and str(asset.get("geometryConfidence") or "").startswith("serialized_")
    ]
    if known_rects:
        min_x = min(rect["x"] for rect in known_rects)
        min_y = min(rect["y"] for rect in known_rects)
        max_x = max(rect["x"] + rect["width"] for rect in known_rects)
        max_y = max(rect["y"] + rect["height"] for rect in known_rects)
        rect = {"x": min_x, "y": min_y, "width": max_x - min_x, "height": max_y - min_y}
        confidence = "serialized_entries"
        z = min(
            int(asset.get("z") or 0)
            for asset in images
            if asset.get("rect") and str(asset.get("geometryConfidence") or "").startswith("serialized_")
        )
    hitboxes = parse_hotspot_entries(meta_component, source_data)
    if hitboxes:
        min_x = min(item["rect"]["x"] for item in hitboxes)
        min_y = min(item["rect"]["y"] for item in hitboxes)
        max_x = max(item["rect"]["x"] + item["rect"]["width"] for item in hitboxes)
        max_y = max(item["rect"]["y"] + item["rect"]["height"] for item in hitboxes)
        rect = {"x": min_x, "y": min_y, "width": max_x - min_x, "height": max_y - min_y}
        confidence = "serialized_entries"
        z = 100
    holder_rect = parse_bitmap_rect(meta_component, source_data) or parse_text_rect(meta_component, source_data) or parse_video_rect(meta_component, source_data)
    if holder_rect and confidence == "unknown":
        rect = holder_rect["rect"]
        confidence = holder_rect["geometryConfidence"]
        geometry_source_offset = holder_rect["sourceOffset"]
    sprite_item_count = parse_sprite_item_count(meta_component, source_data)
    if sprite_item_count is not None and confidence == "unknown":
        rect = {"x": 0, "y": 0, "width": 48, "height": 48}
        confidence = "synthetic_items"
        z = 50
    if component_type == "Sound_Holder":
        confidence = "not_visual"
        rect = {"x": 0, "y": 0, "width": 0, "height": 0}
    elif component_type == "Recorder":
        confidence = "not_visual"
        rect = {"x": 0, "y": 0, "width": 0, "height": 0}
    elif component_type in {"Panorama", "Panorama_Holder"} and confidence == "unknown":
        confidence = "fallback_full_page"
        rect = {"x": 0, "y": 0, "width": 640, "height": 480}
    elif component_type == "Video_Holder" and confidence == "unknown":
        confidence = "fallback_full_page"
        rect = {"x": 0, "y": 0, "width": 640, "height": 480}
    elif component_type == "HotSpot_Holder" and not hitboxes and int(meta_component.get("private_size") or 0) <= 32:
        confidence = "not_visual"
        rect = {"x": 0, "y": 0, "width": 0, "height": 0}
    return {
        "id": f"{meta_component.get('index'):02d}_{safe_id(component_type)}",
        "type": component_type,
        "sourceType": meta_component.get("display_name"),
        "namespace": namespace,
        "index": meta_component.get("index"),
        "z": z if confidence != "unknown" else meta_component.get("index", 0),
        "visible": bool((primary or sprite_item_count is not None) and confidence != "not_visual"),
        "rect": rect,
        "geometryConfidence": confidence,
        "itemCount": sprite_item_count,
        "hitboxes": hitboxes,
        "sourceOffset": meta_component.get("offset"),
        "geometrySourceOffset": geometry_source_offset,
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
        build_component(component, web_root, workspace_root, hints, bdf_data, "Page") for component in meta.get("components", [])
    ]
    # WYBORW keeps two full-height TransparentVideoHolder entries for the
    # opened menu. Their recovered z values are high because they are menu
    # overlays, but the original script reveals them only after the menu
    # hotspot is activated.
    if meta_path.parent.name.upper() == "WYBORW":
        for component in components:
            if component.get("type") != "Transparent_Video_Holder":
                continue
            for asset in component.get("assets") or []:
                if normalized_board_video_id(asset.get("path") or "") in {25, 26}:
                    asset["initiallyVisible"] = False
    attach_puzzle_fallbacks(components, meta_path.parent.name, meta_path.parent.parent, web_root, workspace_root)
    background_bmp = page_dir / "background.bmp"
    palette = page_dir / "background_palette.bin"

    warnings = []
    if promote_full_page_bitmap_background(components, background_bmp.exists()):
        warnings.append("Promoted full-page BitmapHolder asset as background fallback.")
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
        if component.get("type") == "Transparent_Video_Holder"
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


def build_group(meta_path: Path, web_root: Path, workspace_root: Path) -> dict[str, Any]:
    meta = read_json(meta_path)
    source_path = Path(meta.get("source") or "")
    grp_data = source_path.read_bytes() if source_path.exists() else None
    components = [
        build_component(component, web_root, workspace_root, {"MultiBitmap": {}, "Sprite_Holder": {}, "Transparent_Video_Holder": {}}, grp_data, "Group")
        for component in meta.get("components", [])
    ]
    cursors = []
    warnings = []
    for cursor in meta.get("cursors", []):
        pgm = cursor.get("pgm_output")
        if not pgm:
            warnings.append(f"Cursor {cursor.get('index')} ({cursor.get('name') or 'unnamed'}) has no extracted pixel bitmap.")
        preview_url = rel_url(pgm, web_root, workspace_root)
        if pgm:
            pgm_path = Path(pgm)
            preview_rel = Path("assets") / "cursors" / f"{safe_id(meta_path.parent.name)}_{int(cursor.get('index') or 0):03d}.png"
            preview_path = web_root / preview_rel
            try:
                from PIL import Image

                preview_path.parent.mkdir(parents=True, exist_ok=True)
                with Image.open(pgm_path) as image:
                    image.save(preview_path, format="PNG")
                preview_url = preview_rel.as_posix()
            except (ImportError, OSError, ValueError):
                pass
        cursors.append(
            {
                "index": cursor.get("index"),
                "name": cursor.get("name"),
                "width": cursor.get("width"),
                "height": cursor.get("height"),
                "url": preview_url,
                "path": pgm,
                "previewPath": str(preview_path) if pgm and preview_path.exists() else None,
            }
        )
    return {
        "id": meta_path.parent.name,
        "title": meta_path.parent.name,
        "sourceGrp": meta.get("source"),
        "cursors": cursors,
        "components": components,
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
            if asset.get("stillUrl"):
                urls.append(asset["stillUrl"])
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
    groups_dir = web_root / "groups"
    assets_dir = web_root / "assets"
    scenes_dir.mkdir(parents=True, exist_ok=True)
    groups_dir.mkdir(parents=True, exist_ok=True)
    assets_dir.mkdir(parents=True, exist_ok=True)

    metadata_paths = sorted((args.extracted / "bdf").glob("*/metadata.json"))
    available = {path.parent.name.upper(): path for path in metadata_paths}
    order, order_warnings = parse_project_order(args.project, set(available))
    if not order:
        order = sorted(available)

    project_info = parse_project_file(args.project)
    project_block = None
    if project_info is not None:
        global_script = project_info["globalScript"]
        project_block = {
            "version": project_info["version"],
            "startupPage": project_info["startupPage"],
            "audioPresetIndex": project_info["audioPresetIndex"],
            "pageCount": len(project_info["pageNames"]),
            "groupNames": project_info["groupNames"],
            "signatureValid": project_info["signatureValid"],
            "globalScript": global_script,
            "globalScriptHandlers": [
                {"name": match.group(1), "signature": f"{match.group(1)}({match.group(2).strip()})"}
                for match in HANDLER_PATTERN.finditer(global_script)
            ],
        }

    index_entries = []
    validation = {
        "missingAssets": [],
        "componentsWithoutGeometry": 0,
        "scenesWithoutBackgrounds": 0,
        "scriptsFound": 0,
        "scriptsMissing": 0,
        "totalPages": 0,
        "totalGroups": 0,
        "totalGroupComponents": 0,
        "totalGroupAssets": 0,
        "totalComponents": 0,
        "totalAssets": 0,
        "totalDirectAssets": 0,
        "totalDirectBytes": 0,
        "warnings": order_warnings,
    }

    direct_entries = []
    direct_root = args.extracted / "direct"
    if direct_root.exists():
        for asset_path in sorted(path for path in direct_root.rglob("*") if path.is_file()):
            suffix = asset_path.suffix.lower()
            if suffix not in {".wav", ".avi"}:
                continue
            media_type = "audio" if suffix == ".wav" else "video"
            size = asset_path.stat().st_size
            direct_entries.append(
                {
                    "id": f"{media_type}-{safe_id(asset_path.stem)}",
                    "title": asset_path.stem,
                    "name": asset_path.name,
                    "mediaType": media_type,
                    "extension": suffix.removeprefix(".").upper(),
                    "size": size,
                    "path": str(asset_path),
                    "url": rel_url(str(asset_path), web_root, workspace_root),
                }
            )
            validation["totalDirectAssets"] += 1
            validation["totalDirectBytes"] += size

    group_entries = []
    for group_meta_path in sorted((args.extracted / "grp").glob("*/metadata.json")):
        group = build_group(group_meta_path, web_root, workspace_root)
        write_json(groups_dir / f"{group['id']}.json", group)
        component_count = len(group["components"])
        asset_count = sum(len(component["assets"]) + len(component["audio"]) for component in group["components"])
        missing = validate_scene(group, workspace_root)
        validation["missingAssets"].extend({"group": group["id"], "error": item} for item in missing)
        validation["totalGroups"] += 1
        validation["totalGroupComponents"] += component_count
        validation["totalGroupAssets"] += asset_count
        group_entries.append(
            {
                "id": group["id"],
                "title": group["title"],
                "url": f"groups/{group['id']}.json",
                "sourceGrp": group["sourceGrp"],
                "cursorCount": len(group["cursors"]),
                "componentCount": component_count,
                "assetCount": asset_count,
            }
        )

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
        "project": project_block,
        "groupsUrl": "groups/index.json",
        "assetsUrl": "assets/index.json",
        "scenes": index_entries,
        "validation": validation,
    }
    groups_index = {
        "title": "GraphBoard Group Assets",
        "generatedFrom": str(args.extracted / "grp"),
        "groups": group_entries,
        "validation": {
            "totalGroups": validation["totalGroups"],
            "totalGroupComponents": validation["totalGroupComponents"],
            "totalGroupAssets": validation["totalGroupAssets"],
        },
    }
    write_json(scenes_dir / "index.json", index)
    write_json(groups_dir / "index.json", groups_index)
    write_json(
        assets_dir / "index.json",
        {
            "title": "GraphBoard Direct Media",
            "generatedFrom": str(direct_root),
            "assets": direct_entries,
            "validation": {
                "totalDirectAssets": validation["totalDirectAssets"],
                "totalDirectBytes": validation["totalDirectBytes"],
            },
        },
    )
    write_json(web_root / "validation.json", validation)
    print(json.dumps(validation, indent=2, ensure_ascii=False))
    return 1 if validation["missingAssets"] else 0


if __name__ == "__main__":
    raise SystemExit(main())
