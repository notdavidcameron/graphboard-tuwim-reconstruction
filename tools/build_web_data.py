#!/usr/bin/env python3
"""Package a GraphBoard DATA folder for the web port.

Copies the files the wasm runtime actually reads (.PRJ/.BDF/.GRP) into the
output directory with uppercase-normalized names and writes a manifest.json
the frontend loader uses for lazy per-page fetching.

Usage:
  python tools/build_web_data.py "<path to DATA>" web_port_v2/data/tuwim [--link]

--link uses NTFS hard links instead of copies (same-volume dev setups).
"""

import argparse
import json
import os
import shutil
import sys

RUNTIME_EXTENSIONS = {".PRJ", ".BDF", ".GRP"}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", help="original game DATA directory")
    parser.add_argument("dest", help="output directory (served statically)")
    parser.add_argument("--link", action="store_true",
                        help="hard-link instead of copy (same NTFS volume)")
    args = parser.parse_args()

    if not os.path.isdir(args.source):
        print(f"error: source is not a directory: {args.source}", file=sys.stderr)
        return 1
    os.makedirs(args.dest, exist_ok=True)

    entries = []
    total = 0
    for name in sorted(os.listdir(args.source)):
        src = os.path.join(args.source, name)
        if not os.path.isfile(src):
            continue
        ext = os.path.splitext(name)[1].upper()
        if ext not in RUNTIME_EXTENSIONS:
            continue
        canonical = name.upper()
        dst = os.path.join(args.dest, canonical)
        size = os.path.getsize(src)
        if not (os.path.exists(dst) and os.path.getsize(dst) == size):
            if os.path.exists(dst):
                os.remove(dst)
            if args.link:
                try:
                    os.link(src, dst)
                except OSError:
                    shutil.copyfile(src, dst)
            else:
                shutil.copyfile(src, dst)
        entries.append({"file": canonical, "size": size})
        total += size

    manifest_path = os.path.join(args.dest, "manifest.json")
    with open(manifest_path, "w", encoding="utf-8") as fh:
        json.dump({"files": entries}, fh, indent=1)

    print(f"packaged {len(entries)} files, {total / (1 << 20):.1f} MB -> {args.dest}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
