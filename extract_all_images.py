#!/usr/bin/env python3
"""Refresh and collect GraphBoard still-image assets."""

from __future__ import annotations

import argparse
import json
import shutil
import struct
import sys
import time
from pathlib import Path

from graphboard_extract_assets import extract_bdf, parse_grp


VIEWABLE_EXTENSIONS = {".bmp", ".dib", ".gif", ".pgm", ".png", ".jpg", ".jpeg"}
RAW_EXTENSIONS = {".raw"}


def log(message: str) -> None:
    print(message, flush=True)


def safe_part(value: str) -> str:
    return "".join(ch if ch.isalnum() or ch in "._- " else "_" for ch in value).strip() or "unnamed"


def unique_path(path: Path) -> Path:
    if not path.exists():
        return path
    stem = path.stem
    suffix = path.suffix
    parent = path.parent
    index = 1
    while True:
        candidate = parent / f"{stem}_{index:03d}{suffix}"
        if not candidate.exists():
            return candidate
        index += 1


def copy_collected(src: Path, root: Path, dest_root: Path) -> Path:
    rel = src.relative_to(root)
    dest_name = "__".join(safe_part(part) for part in rel.parts)
    dest = unique_path(dest_root / dest_name)
    dest.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(src, dest)
    return dest


def scan_embedded_signatures(source_dir: Path, output_dir: Path) -> list[dict]:
    """Conservative direct image signature scan across original DATA files."""
    out_dir = output_dir / "signature_images"
    out_dir.mkdir(parents=True, exist_ok=True)
    found: list[dict] = []
    files = sorted(path for path in source_dir.iterdir() if path.is_file())
    for path in files:
        data = path.read_bytes()

        offset = 0
        while True:
            marker = data.find(b"BM", offset)
            if marker == -1:
                break
            size = plausible_bmp(data, marker)
            if size is not None:
                dest = unique_path(out_dir / f"{path.stem}_{marker:08x}.bmp")
                dest.write_bytes(data[marker : marker + size])
                found.append({"source": str(path), "offset": marker, "kind": "bmp_signature", "path": str(dest)})
                offset = marker + size
                continue
            offset = marker + 2

        for sig, ext in ((b"GIF87a", ".gif"), (b"GIF89a", ".gif")):
            offset = 0
            while True:
                marker = data.find(sig, offset)
                if marker == -1:
                    break
                size = plausible_gif(data, marker)
                if size is not None:
                    dest = unique_path(out_dir / f"{path.stem}_{marker:08x}{ext}")
                    dest.write_bytes(data[marker : marker + size])
                    found.append({"source": str(path), "offset": marker, "kind": "gif_signature", "path": str(dest)})
                    offset = marker + size
                else:
                    offset = marker + len(sig)

        offset = 0
        png_sig = b"\x89PNG\r\n\x1a\n"
        while True:
            marker = data.find(png_sig, offset)
            if marker == -1:
                break
            size = plausible_png(data, marker)
            if size is not None:
                dest = unique_path(out_dir / f"{path.stem}_{marker:08x}.png")
                dest.write_bytes(data[marker : marker + size])
                found.append({"source": str(path), "offset": marker, "kind": "png_signature", "path": str(dest)})
                offset = marker + size
            else:
                offset = marker + len(png_sig)

        offset = 0
        while True:
            marker = data.find(b"\xff\xd8\xff", offset)
            if marker == -1:
                break
            size = plausible_jpeg(data, marker)
            if size is not None:
                dest = unique_path(out_dir / f"{path.stem}_{marker:08x}.jpg")
                dest.write_bytes(data[marker : marker + size])
                found.append({"source": str(path), "offset": marker, "kind": "jpeg_signature", "path": str(dest)})
                offset = marker + size
            else:
                offset = marker + 3

    return found


def plausible_bmp(data: bytes, marker: int) -> int | None:
    if marker + 54 > len(data) or data[marker : marker + 2] != b"BM":
        return None
    size = struct.unpack_from("<I", data, marker + 2)[0]
    reserved1 = struct.unpack_from("<H", data, marker + 6)[0]
    reserved2 = struct.unpack_from("<H", data, marker + 8)[0]
    pixel_offset = struct.unpack_from("<I", data, marker + 10)[0]
    if reserved1 or reserved2 or not (54 <= size <= len(data) - marker):
        return None
    dib_size = struct.unpack_from("<I", data, marker + 14)[0]
    if dib_size not in {40, 52, 56, 108, 124}:
        return None
    if marker + 14 + dib_size > len(data):
        return None
    width = struct.unpack_from("<i", data, marker + 18)[0]
    height = struct.unpack_from("<i", data, marker + 22)[0]
    planes = struct.unpack_from("<H", data, marker + 26)[0]
    bit_count = struct.unpack_from("<H", data, marker + 28)[0]
    if planes != 1 or bit_count not in {1, 4, 8, 16, 24, 32}:
        return None
    if not (1 <= abs(width) <= 4096 and 1 <= abs(height) <= 4096):
        return None
    if not (14 + dib_size <= pixel_offset < size):
        return None
    return size


def plausible_gif(data: bytes, marker: int) -> int | None:
    if marker + 13 > len(data) or data[marker : marker + 3] != b"GIF":
        return None
    width = struct.unpack_from("<H", data, marker + 6)[0]
    height = struct.unpack_from("<H", data, marker + 8)[0]
    if not (1 <= width <= 4096 and 1 <= height <= 4096):
        return None
    trailer = data.find(b"\x3b", marker + 13)
    if trailer == -1:
        return None
    size = trailer - marker + 1
    if size < 20 or size > 50_000_000:
        return None
    return size


def plausible_png(data: bytes, marker: int) -> int | None:
    png_sig = b"\x89PNG\r\n\x1a\n"
    if data[marker : marker + len(png_sig)] != png_sig:
        return None
    if marker + 33 > len(data):
        return None
    if data[marker + 12 : marker + 16] != b"IHDR":
        return None
    width = struct.unpack_from(">I", data, marker + 16)[0]
    height = struct.unpack_from(">I", data, marker + 20)[0]
    if not (1 <= width <= 4096 and 1 <= height <= 4096):
        return None
    iend = data.find(b"IEND", marker + 33)
    if iend == -1 or iend + 8 > len(data):
        return None
    return iend - marker + 8


def plausible_jpeg(data: bytes, marker: int) -> int | None:
    if marker + 12 > len(data) or data[marker : marker + 3] != b"\xff\xd8\xff":
        return None
    header = data[marker + 6 : marker + 12]
    if not (header.startswith(b"JFIF") or header.startswith(b"Exif")):
        return None
    end = data.find(b"\xff\xd9", marker + 12)
    if end == -1:
        return None
    size = end - marker + 2
    if size < 32 or size > 50_000_000:
        return None
    return size


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    parser.add_argument("-o", "--output", type=Path, default=Path("extracted_assets"))
    parser.add_argument("--no-refresh", action="store_true", help="Only collect files that already exist")
    parser.add_argument("--clean-collection", action="store_true", help="Remove all_images/all_images_raw/signature_images first")
    args = parser.parse_args()

    started = time.time()
    if args.clean_collection:
        for name in ("all_images", "all_images_raw", "signature_images"):
            target = args.output / name
            if target.exists():
                log(f"[clean] {target}")
                shutil.rmtree(target)

    errors: list[dict] = []
    if not args.no_refresh:
        bdfs = sorted(args.source.glob("*.BDF"))
        log(f"[refresh] BDF files={len(bdfs)}")
        for index, path in enumerate(bdfs, 1):
            try:
                meta = extract_bdf(path, args.output)
                image_outputs = [
                    item
                    for item in meta.get("outputs", [])
                    if Path(str(item.get("path", ""))).suffix.lower() in VIEWABLE_EXTENSIONS | RAW_EXTENSIONS
                ]
                log(f"[refresh] {index:03d}/{len(bdfs):03d} {path.name} image_outputs={len(image_outputs)}")
            except Exception as exc:
                log(f"[refresh] {index:03d}/{len(bdfs):03d} {path.name} ERROR {exc!r}")
                errors.append({"source": str(path), "error": repr(exc)})

        grps = sorted(args.source.glob("*.GRP"))
        log(f"[refresh] GRP files={len(grps)}")
        for index, path in enumerate(grps, 1):
            try:
                meta = parse_grp(path, args.output)
                log(f"[refresh] grp {index:03d}/{len(grps):03d} {path.name} outputs={len(meta.get('outputs', []))}")
            except Exception as exc:
                log(f"[refresh] grp {index:03d}/{len(grps):03d} {path.name} ERROR {exc!r}")
                errors.append({"source": str(path), "error": repr(exc)})

    log("[scan] direct image signatures")
    signature_hits = scan_embedded_signatures(args.source, args.output)
    log(f"[scan] signature_hits={len(signature_hits)}")

    viewable_root = args.output / "all_images"
    raw_root = args.output / "all_images_raw"
    viewable_root.mkdir(parents=True, exist_ok=True)
    raw_root.mkdir(parents=True, exist_ok=True)

    collected_viewable: list[dict] = []
    collected_raw: list[dict] = []
    search_roots = [args.output / "bdf", args.output / "grp", args.output / "signature_images"]
    for root in search_roots:
        if not root.exists():
            continue
        for src in sorted(path for path in root.rglob("*") if path.is_file()):
            ext = src.suffix.lower()
            if src.is_relative_to(viewable_root) or src.is_relative_to(raw_root):
                continue
            if ext in VIEWABLE_EXTENSIONS:
                dest = copy_collected(src, args.output, viewable_root)
                collected_viewable.append({"source": str(src), "path": str(dest), "extension": ext})
            elif ext in RAW_EXTENSIONS:
                dest = copy_collected(src, args.output, raw_root)
                collected_raw.append({"source": str(src), "path": str(dest), "extension": ext})

    by_extension: dict[str, int] = {}
    for item in collected_viewable:
        by_extension[item["extension"]] = by_extension.get(item["extension"], 0) + 1

    raw_by_extension: dict[str, int] = {}
    for item in collected_raw:
        raw_by_extension[item["extension"]] = raw_by_extension.get(item["extension"], 0) + 1

    summary = {
        "source": str(args.source),
        "output": str(args.output),
        "viewable_count": len(collected_viewable),
        "raw_count": len(collected_raw),
        "viewable_by_extension": dict(sorted(by_extension.items())),
        "raw_by_extension": dict(sorted(raw_by_extension.items())),
        "signature_hits": signature_hits,
        "errors": errors,
        "elapsed_seconds": round(time.time() - started, 1),
    }
    summary_path = args.output / "all_images_summary.json"
    summary_path.write_text(json.dumps(summary, indent=2, ensure_ascii=False), encoding="utf-8")
    log(f"[summary] {json.dumps(summary, ensure_ascii=False)}")
    log(f"[summary] wrote {summary_path}")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
