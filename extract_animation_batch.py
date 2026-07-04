#!/usr/bin/env python3
"""Refresh generated animation assets with progress logging."""

from __future__ import annotations

import argparse
import json
import shutil
import sys
import time
from pathlib import Path

from graphboard_extract_assets import extract_bdf


def log(message: str) -> None:
    print(message, flush=True)


def count_files(path: Path, pattern: str) -> int:
    if not path.exists():
        return 0
    return sum(1 for _ in path.rglob(pattern))


def remove_tree(path: Path) -> None:
    if path.exists():
        shutil.rmtree(path)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    parser.add_argument("-o", "--output", type=Path, default=Path("extracted_assets"))
    parser.add_argument("--batch-size", type=int, default=25)
    parser.add_argument("--start", type=int, default=0)
    parser.add_argument("--clean-first", action="store_true")
    parser.add_argument("--include-raw", action="store_true")
    args = parser.parse_args()

    bdfs = sorted(args.source.glob("*.BDF"))
    if args.start:
        bdfs = bdfs[args.start :]
    if args.batch_size > 0:
        bdfs = bdfs[: args.batch_size]

    out_bdf = args.output / "bdf"
    if args.clean_first:
        log(f"[clean] removing generated animation folders under {out_bdf}")
        for anim_dir in out_bdf.rglob("animations"):
            log(f"[clean] {anim_dir}")
            remove_tree(anim_dir)
        for extra_name in ("board_video_probe", "multibitmap_probe"):
            for extra_dir in out_bdf.rglob(extra_name):
                log(f"[clean] {extra_dir}")
                remove_tree(extra_dir)

    log(f"[start] source={args.source}")
    log(f"[start] output={args.output}")
    log(f"[start] files={len(bdfs)} start={args.start} batch_size={args.batch_size}")

    failures: list[dict[str, str]] = []
    started = time.time()
    for ordinal, path in enumerate(bdfs, 1):
        page_dir = args.output / "bdf" / path.stem
        before_gifs = count_files(page_dir / "animations", "*.gif")
        log(f"[{ordinal:03d}/{len(bdfs):03d}] {path.name} begin, old_gifs={before_gifs}")
        item_start = time.time()
        try:
            meta = extract_bdf(path, args.output, include_raw=args.include_raw)
        except Exception as exc:
            failures.append({"source": str(path), "error": repr(exc)})
            log(f"[{ordinal:03d}/{len(bdfs):03d}] {path.name} ERROR {exc!r}")
            continue

        outputs = meta.get("outputs", [])
        gifs = [item for item in outputs if item.get("kind") in {"board_video_gif", "multibitmap_gif"}]
        wavs = [item for item in outputs if item.get("kind") == "board_video_pcm_wave"]
        elapsed = time.time() - item_start
        log(
            f"[{ordinal:03d}/{len(bdfs):03d}] {path.name} done "
            f"gifs={len(gifs)} board_wavs={len(wavs)} outputs={len(outputs)} seconds={elapsed:.1f}"
        )
        for item in gifs:
            log(f"    gif: {item.get('path')}")

    total_gifs = count_files(out_bdf, "*.gif")
    total_wavs = count_files(out_bdf, "*.wav")
    summary = {
        "source": str(args.source),
        "output": str(args.output),
        "processed": len(bdfs),
        "start": args.start,
        "batch_size": args.batch_size,
        "total_gifs_under_bdf": total_gifs,
        "total_wavs_under_bdf": total_wavs,
        "failures": failures,
        "elapsed_seconds": round(time.time() - started, 1),
    }
    summary_path = args.output / "animation_batch_summary.json"
    summary_path.write_text(json.dumps(summary, indent=2, ensure_ascii=False), encoding="utf-8")
    log(f"[summary] {json.dumps(summary, ensure_ascii=False)}")
    log(f"[summary] wrote {summary_path}")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
