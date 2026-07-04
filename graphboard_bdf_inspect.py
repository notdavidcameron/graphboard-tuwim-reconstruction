#!/usr/bin/env python3
"""Inspect GraphBoard .BDF page files.

This is intentionally conservative: it parses the host GraphBoard wrapper
records and reports component-private payload boundaries when they can be
anchored by the next validated wrapper or by the page script block.
"""

from __future__ import annotations

import argparse
import json
import struct
from pathlib import Path


def read_u8(data: bytes, offset: int) -> tuple[int, int]:
    return data[offset], offset + 1


def read_u16(data: bytes, offset: int) -> tuple[int, int]:
    return struct.unpack_from("<H", data, offset)[0], offset + 2


def read_u32(data: bytes, offset: int) -> tuple[int, int]:
    return struct.unpack_from("<I", data, offset)[0], offset + 4


def read_i32(data: bytes, offset: int) -> tuple[int, int]:
    return struct.unpack_from("<i", data, offset)[0], offset + 4


def read_guid(data: bytes, offset: int) -> tuple[str, int]:
    raw = data[offset : offset + 16]
    d1, d2, d3 = struct.unpack_from("<IHH", raw, 0)
    tail = raw[8:]
    guid = f"{d1:08X}-{d2:04X}-{d3:04X}-{tail[0]:02X}{tail[1]:02X}-{tail[2:].hex().upper()}"
    return guid, offset + 16


def read_mfc_cstring(data: bytes, offset: int) -> tuple[str, int, int]:
    start = offset
    first, offset = read_u8(data, offset)
    if first < 0xFF:
        length = first
    else:
        length, offset = read_u16(data, offset)
        if length == 0xFFFF:
            length, offset = read_u32(data, offset)

    raw = data[offset : offset + length]
    return raw.decode("cp1250", "replace"), offset + length, offset - start + length


def parse_wrapper(data: bytes, offset: int) -> dict:
    start = offset
    clsid, offset = read_guid(data, offset)
    version, offset = read_u32(data, offset)
    function_count, offset = read_u32(data, offset)
    display_name, offset, _ = read_mfc_cstring(data, offset)
    property_count, offset = read_u32(data, offset)
    duplicate_clsid, offset = read_guid(data, offset)

    functions = []
    for _ in range(function_count):
        dispatch_id, offset = read_u32(data, offset)
        raw_name_len, offset = read_u32(data, offset)
        raw_name = data[offset : offset + raw_name_len + 1]
        offset += raw_name_len + 1
        return_type, offset, _ = read_mfc_cstring(data, offset)
        name, offset, _ = read_mfc_cstring(data, offset)
        description, offset, _ = read_mfc_cstring(data, offset)
        flags, offset = read_u16(data, offset)
        functions.append(
            {
                "dispatch_id": dispatch_id,
                "raw_name_hex": raw_name.hex(),
                "return_type": return_type,
                "name": name,
                "description": description,
                "flags": flags,
            }
        )

    properties = []
    for _ in range(property_count):
        raw_name_len, offset = read_u32(data, offset)
        raw_name = data[offset : offset + raw_name_len + 1]
        offset += raw_name_len + 1
        variant_or_dispatch, offset = read_u32(data, offset)
        name, offset, _ = read_mfc_cstring(data, offset)
        flags, offset = read_u16(data, offset)
        properties.append(
            {
                "raw_name_hex": raw_name.hex(),
                "variant_or_dispatch": variant_or_dispatch,
                "name": name,
                "flags": flags,
            }
        )

    return {
        "offset": start,
        "clsid": clsid,
        "version": version,
        "display_name": display_name,
        "duplicate_clsid": duplicate_clsid,
        "function_count": function_count,
        "property_count": property_count,
        "private_offset": offset,
        "functions": functions,
        "properties": properties,
    }


def plausible_wrapper(data: bytes, offset: int) -> bool:
    try:
        parsed = parse_wrapper(data, offset)
    except (IndexError, struct.error, UnicodeDecodeError):
        return False
    if parsed["version"] != 1:
        return False
    if parsed["clsid"] == "00000000-0000-0000-0000-000000000000":
        return False
    if parsed["clsid"] != parsed["duplicate_clsid"]:
        return False
    if not (0 <= parsed["function_count"] < 80 and 0 <= parsed["property_count"] < 100):
        return False
    if not parsed["display_name"]:
        return False
    return all(32 <= ord(ch) < 127 for ch in parsed["display_name"])


def find_next_wrappers(data: bytes, start: int, count: int) -> list[int]:
    offsets = []
    cursor = start
    while len(offsets) < count and cursor < len(data) - 64:
        if data[cursor + 16 : cursor + 20] == b"\x01\x00\x00\x00" and plausible_wrapper(data, cursor):
            offsets.append(cursor)
            cursor = parse_wrapper(data, cursor)["private_offset"]
        else:
            cursor += 1
    return offsets


def find_script_block(data: bytes) -> dict | None:
    marker = data.find(b"//Script code")
    if marker < 0:
        return None
    start = data.rfind(b"//", 0, marker)
    if start < 7:
        return None
    if data[start - 7 : start - 3] != b"\x01\x00\x00\x00" or data[start - 3] != 0xFF:
        return {"text_marker_offset": marker}
    length = struct.unpack_from("<H", data, start - 2)[0]
    text_end = start + length
    result = {
        "version_offset": start - 7,
        "version": 1,
        "string_prefix_offset": start - 3,
        "text_offset": start,
        "text_length": length,
        "text_end": text_end,
    }
    engine = parse_script_engine_state(data, text_end)
    if engine:
        result["engine_state"] = engine
    return result


def parse_script_engine_state(data: bytes, offset: int) -> dict | None:
    try:
        start = offset
        version, offset = read_u32(data, offset)
        if not (1 <= version <= 4):
            return None
        block_count, offset = read_u32(data, offset)
        if block_count > 100:
            return None
        handler_offsets = {}
        for name in ("field_38", "field_3c", "field_30", "field_34"):
            handler_offsets[name], offset = read_u32(data, offset)

        blocks = []
        for index in range(block_count):
            block_offset = offset
            start_offset, offset = read_u32(data, offset)
            duplicate_start_offset, offset = read_u32(data, offset)
            case_count, offset = read_u32(data, offset)
            end_or_body_offset, offset = read_u32(data, offset)
            aux_offset, offset = read_u32(data, offset)
            if case_count > 256:
                return None
            cases = []
            for _ in range(case_count):
                value, offset = read_u32(data, offset)
                target, offset = read_u32(data, offset)
                cases.append({"value": value, "target_offset": target})
            blocks.append(
                {
                    "index": index,
                    "file_offset": block_offset,
                    "start_offset": start_offset,
                    "duplicate_start_offset": duplicate_start_offset,
                    "case_count": case_count,
                    "end_or_body_offset": end_or_body_offset,
                    "aux_offset": aux_offset,
                    "cases": cases,
                }
            )

        table_count, offset = read_u32(data, offset)
        if table_count > 100:
            return None
        table_a = []
        table_b = []
        for _ in range(table_count):
            value, offset = read_u32(data, offset)
            table_a.append(value)
        for _ in range(table_count):
            value, offset = read_u32(data, offset)
            table_b.append(value)

        versioned_tail = {}
        if version > 1:
            for name in ("field_44", "field_48", "field_4c", "field_50"):
                versioned_tail[name], offset = read_u32(data, offset)
        if version > 2:
            versioned_tail["field_54"], offset = read_u32(data, offset)
        if version > 3:
            versioned_tail["field_58"], offset = read_u32(data, offset)

        return {
            "offset": start,
            "version": version,
            "block_count": block_count,
            "handler_offsets": handler_offsets,
            "blocks": blocks,
            "table_count": table_count,
            "table_a": table_a,
            "table_b": table_b,
            "versioned_tail": versioned_tail,
            "end_offset": offset,
        }
    except (IndexError, struct.error):
        return None


def inspect_bdf(path: Path) -> dict:
    data = path.read_bytes()
    offset = 0
    banner = data[:100].split(b"\x00", 1)[0].decode("ascii", "replace")
    offset = 100
    version, offset = read_u32(data, offset)

    rect = []
    for _ in range(4):
        value, offset = read_i32(data, offset)
        rect.append(value)
    min_layer, offset = read_i32(data, offset)
    max_layer, offset = read_i32(data, offset)
    background_flag, offset = read_u32(data, offset)
    background_color_index, offset = read_u8(data, offset)
    palette_size, offset = read_u32(data, offset)
    offset += palette_size
    dib_size, offset = read_u32(data, offset)
    offset += dib_size

    component_list_offset = offset
    component_list_version, offset = read_u32(data, offset)
    component_count, offset = read_u32(data, offset)
    component_offsets = find_next_wrappers(data, offset, component_count)
    components = []
    for index, component_offset in enumerate(component_offsets):
        wrapper = parse_wrapper(data, component_offset)
        next_offset = component_offsets[index + 1] if index + 1 < len(component_offsets) else None
        components.append(
            {
                "index": index,
                "offset": component_offset,
                "clsid": wrapper["clsid"],
                "display_name": wrapper["display_name"],
                "function_count": wrapper["function_count"],
                "property_count": wrapper["property_count"],
                "private_offset": wrapper["private_offset"],
                "private_length_to_next_wrapper": None
                if next_offset is None
                else next_offset - wrapper["private_offset"],
                "last_functions": [item["name"] for item in wrapper["functions"][-4:]],
                "last_properties": [item["name"] for item in wrapper["properties"][-6:]],
            }
        )

    script = find_script_block(data)
    if script and components:
        last = components[-1]
        if script.get("version_offset", 0) > last["private_offset"]:
            last["private_length_to_script_editor"] = script["version_offset"] - last["private_offset"]

    return {
        "path": str(path),
        "file_size": len(data),
        "banner": banner,
        "version": version,
        "page_rect": rect,
        "layer_range": [min_layer, max_layer],
        "background_flag": background_flag,
        "background_color_index": background_color_index,
        "palette_size": palette_size,
        "dib_size": dib_size,
        "component_list_offset": component_list_offset,
        "component_list_version": component_list_version,
        "component_count": component_count,
        "components": components,
        "script": script,
    }


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("bdf", type=Path)
    parser.add_argument("--indent", type=int, default=2)
    args = parser.parse_args()
    print(json.dumps(inspect_bdf(args.bdf), indent=args.indent))


if __name__ == "__main__":
    main()
