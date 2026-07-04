#!/usr/bin/env python3
"""Convert collected GraphBoard images to JPEG stills."""

from __future__ import annotations

import argparse
import json
import shutil
import sys
import time
from pathlib import Path

from PIL import Image, ImageSequence


INPUT_EXTENSIONS = {".bmp", ".dib", ".gif", ".jpg", ".jpeg", ".pgm", ".png"}


def log(message: str) -> None:
    print(message, flush=True)


def jpeg_ready(image: Image.Image) -> Image.Image:
    """Return an RGB image suitable for JPEG output."""
    if image.mode in {"RGBA", "LA"} or (image.mode == "P" and "transparency" in image.info):
        rgba = image.convert("RGBA")
        background = Image.new("RGBA", rgba.size, (255, 255, 255, 255))
        background.alpha_composite(rgba)
        return background.convert("RGB")
    if image.mode != "RGB":
        return image.convert("RGB")
    return image


def unique_path(path: Path) -> Path:
    if not path.exists():
        return path
    index = 1
    while True:
        candidate = path.with_name(f"{path.stem}_{index:03d}{path.suffix}")
        if not candidate.exists():
            return candidate
        index += 1


def convert_one(source: Path, destination: Path, quality: int) -> dict:
    with Image.open(source) as image:
        frame_count = getattr(image, "n_frames", 1)
        if source.suffix.lower() == ".gif":
            image = next(ImageSequence.Iterator(image))
        rgb = jpeg_ready(image)
        destination.parent.mkdir(parents=True, exist_ok=True)
        rgb.save(destination, "JPEG", quality=quality, optimize=True, progressive=False)
        return {
            "source": str(source),
            "path": str(destination),
            "source_extension": source.suffix.lower(),
            "width": rgb.width,
            "height": rgb.height,
            "source_frame_count": frame_count,
            "note": "first frame only" if frame_count > 1 else None,
        }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--input",
        type=Path,
        default=Path("extracted_assets") / "all_images",
        help="Folder containing collected viewable images",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=Path("extracted_assets") / "all_images_jpeg",
        help="Folder to write JPEG files",
    )
    parser.add_argument("--quality", type=int, default=95)
    parser.add_argument("--clean", action="store_true")
    args = parser.parse_args()

    started = time.time()
    if args.clean and args.output.exists():
        log(f"[clean] {args.output}")
        shutil.rmtree(args.output)
    args.output.mkdir(parents=True, exist_ok=True)

    inputs = sorted(path for path in args.input.iterdir() if path.is_file() and path.suffix.lower() in INPUT_EXTENSIONS)
    log(f"[start] input={args.input}")
    log(f"[start] output={args.output}")
    log(f"[start] files={len(inputs)} quality={args.quality}")

    converted: list[dict] = []
    errors: list[dict] = []
    for index, source in enumerate(inputs, 1):
        destination = unique_path(args.output / f"{source.stem}.jpg")
        log(f"[{index:03d}/{len(inputs):03d}] {source.name}")
        try:
            item = convert_one(source, destination, args.quality)
            converted.append(item)
            suffix = f", {item['note']}" if item.get("note") else ""
            log(f"    -> {destination.name} {item['width']}x{item['height']}{suffix}")
        except Exception as exc:
            errors.append({"source": str(source), "error": repr(exc)})
            log(f"    ERROR {exc!r}")

    by_source_extension: dict[str, int] = {}
    for item in converted:
        ext = item["source_extension"]
        by_source_extension[ext] = by_source_extension.get(ext, 0) + 1

    summary = {
        "input": str(args.input),
        "output": str(args.output),
        "converted_count": len(converted),
        "error_count": len(errors),
        "by_source_extension": dict(sorted(by_source_extension.items())),
        "errors": errors,
        "elapsed_seconds": round(time.time() - started, 1),
    }
    manifest = {
        "summary": summary,
        "images": converted,
    }
    manifest_path = args.output / "manifest.json"
    manifest_path.write_text(json.dumps(manifest, indent=2, ensure_ascii=False), encoding="utf-8")
    log(f"[summary] {json.dumps(summary, ensure_ascii=False)}")
    log(f"[summary] wrote {manifest_path}")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
