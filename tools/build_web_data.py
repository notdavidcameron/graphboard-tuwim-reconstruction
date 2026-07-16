#!/usr/bin/env python3
"""Package a GraphBoard DATA folder for the web port.

Copies the files the wasm runtime actually reads (.PRJ/.BDF/.GRP plus the
RIFF/WAVE poem narration stored as .EXS), transcodes the title's Indeo AVI files
to browser-decodable MP4, and writes manifest.json for lazy page and media
loading.

Usage:
  python tools/build_web_data.py "<path to DATA>" web_port_v2/data/tuwim [--link]

--link uses NTFS hard links instead of copies (same-volume dev setups).
"""

import argparse
import json
import os
import shutil
import struct
import subprocess
import sys

RUNTIME_EXTENSIONS = {".PRJ", ".BDF", ".GRP", ".EXS"}


def find_ffmpeg(explicit: str | None) -> str | None:
    if explicit:
        return explicit
    found = shutil.which("ffmpeg")
    if found:
        return found
    try:
        import imageio_ffmpeg  # type: ignore
        return imageio_ffmpeg.get_ffmpeg_exe()
    except (ImportError, RuntimeError):
        return None


def avi_video_info(path: str) -> dict:
    """Read the first AVI video strh/strf pair without requiring ffprobe."""
    data = open(path, "rb").read(1 << 20)
    offset = 0
    while True:
        pos = data.find(b"strh", offset)
        if pos < 0 or pos + 64 > len(data):
            raise ValueError(f"AVI video stream header not found: {path}")
        size = struct.unpack_from("<I", data, pos + 4)[0]
        header = data[pos + 8:pos + 8 + size]
        if len(header) >= 40 and header[:4] == b"vids":
            scale = struct.unpack_from("<I", header, 20)[0]
            rate = struct.unpack_from("<I", header, 24)[0]
            frames = struct.unpack_from("<I", header, 32)[0]
            fmt = data.find(b"strf", pos + 8 + size)
            width = height = 0
            if fmt >= 0 and fmt + 20 <= len(data):
                width = abs(struct.unpack_from("<i", data, fmt + 12)[0])
                height = abs(struct.unpack_from("<i", data, fmt + 16)[0])
            return {
                "fps": rate / scale if scale else 0,
                "frames": frames,
                "width": width,
                "height": height,
            }
        offset = pos + 8 + size


def transcode_avi(ffmpeg: str, source: str, target: str, audio_target: str) -> None:
    os.makedirs(os.path.dirname(target), exist_ok=True)
    if all(os.path.exists(path) and os.path.getmtime(path) >= os.path.getmtime(source)
           for path in (target, audio_target)):
        return
    video_command = [
        ffmpeg, "-hide_banner", "-loglevel", "error", "-y", "-i", source,
        "-map", "0:v:0", "-an", "-c:v", "libx264", "-preset", "medium",
        "-crf", "20", "-pix_fmt", "yuv420p",
        "-movflags", "+faststart", target,
    ]
    audio_command = [
        ffmpeg, "-hide_banner", "-loglevel", "error", "-y", "-i", source,
        "-map", "0:a:0", "-vn", "-c:a", "aac", "-b:a", "96k",
        "-movflags", "+faststart", audio_target,
    ]
    subprocess.run(video_command, check=True)
    subprocess.run(audio_command, check=True)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", help="original game DATA directory")
    parser.add_argument("dest", help="output directory (served statically)")
    parser.add_argument("--link", action="store_true",
                        help="hard-link instead of copy (same NTFS volume)")
    parser.add_argument("--ffmpeg", help="ffmpeg executable used for AVI -> MP4")
    parser.add_argument("--no-video", action="store_true",
                        help="omit external Video_Holder media")
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

    avi_names = [name for name in sorted(os.listdir(args.source))
                 if os.path.isfile(os.path.join(args.source, name)) and
                 os.path.splitext(name)[1].upper() == ".AVI"]
    video_total = 0
    if avi_names and not args.no_video:
        ffmpeg = find_ffmpeg(args.ffmpeg)
        if not ffmpeg:
            print("error: ffmpeg is required for external AVI videos; pass --ffmpeg or --no-video",
                  file=sys.stderr)
            return 1
        for name in avi_names:
            src = os.path.join(args.source, name)
            canonical = name.upper()
            media_rel = "video/" + os.path.splitext(canonical)[0] + ".mp4"
            audio_rel = "video/" + os.path.splitext(canonical)[0] + ".m4a"
            dst = os.path.join(args.dest, *media_rel.split("/"))
            audio_dst = os.path.join(args.dest, *audio_rel.split("/"))
            print(f"video: {canonical} -> {media_rel}")
            transcode_avi(ffmpeg, src, dst, audio_dst)
            info = avi_video_info(src)
            entries.append({
                "file": canonical,
                "size": os.path.getsize(src),
                "media": media_rel,
                "audio": audio_rel,
                **info,
            })
            video_total += os.path.getsize(dst) + os.path.getsize(audio_dst)

    manifest_path = os.path.join(args.dest, "manifest.json")
    with open(manifest_path, "w", encoding="utf-8") as fh:
        json.dump({"files": entries}, fh, indent=1)

    print(f"packaged {len(entries)} files, {total / (1 << 20):.1f} MB runtime + "
          f"{video_total / (1 << 20):.1f} MB video -> {args.dest}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
