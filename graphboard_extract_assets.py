#!/usr/bin/env python3
"""Extract practical assets from a GraphBoard DATA folder.

The extractor is intentionally conservative. It copies direct RIFF/AVI/WAVE
assets, extracts page scripts and embedded RIFF chunks from BDF files, converts
stored DIB backgrounds to BMP, and writes metadata for component-private blocks.
Unknown component-private data is not dumped by default; use --include-raw to
preserve those blobs as well.
"""

from __future__ import annotations

import argparse
import json
import re
import shutil
import struct
import wave
import zlib
from pathlib import Path

from graphboard_bdf_inspect import (
    find_next_wrappers,
    find_script_block,
    parse_script_engine_state,
    parse_wrapper,
    read_i32,
    read_u8,
    read_u32,
)


def safe_name(name: str) -> str:
    clean = re.sub(r"[^A-Za-z0-9._ -]+", "_", name).strip(" .")
    if re.fullmatch(r"(?i)(con|prn|aux|nul|com[1-9]|lpt[1-9])", clean or ""):
        clean = f"_{clean}"
    return clean or "unnamed"


def mkdir(path: Path) -> None:
    path.mkdir(parents=True, exist_ok=True)


def write_json(path: Path, payload: object) -> None:
    mkdir(path.parent)
    path.write_text(json.dumps(payload, indent=2, ensure_ascii=False), encoding="utf-8")


def riff_chunk_size(data: bytes, offset: int) -> int | None:
    if offset + 12 > len(data) or data[offset : offset + 4] != b"RIFF":
        return None
    size = struct.unpack_from("<I", data, offset + 4)[0] + 8
    if size <= 12 or offset + size > len(data):
        return None
    return size


def riff_extension(data: bytes, offset: int) -> str:
    kind = data[offset + 8 : offset + 12]
    if kind == b"WAVE":
        return ".wav"
    if kind == b"AVI ":
        return ".avi"
    return ".riff"


def write_pcm_wave(path: Path, pcm: bytes, sample_rate: int, bits_per_sample: int, channels: int) -> None:
    if bits_per_sample % 8 != 0:
        raise ValueError(f"unsupported PCM width: {bits_per_sample} bits")
    mkdir(path.parent)
    with wave.open(str(path), "wb") as handle:
        handle.setnchannels(channels)
        handle.setsampwidth(bits_per_sample // 8)
        handle.setframerate(sample_rate)
        handle.writeframes(pcm)


def find_board_video_streams(data: bytes, start: int, end: int) -> list[int]:
    markers = []
    offset = data.find(b"Board Video File", start, end)
    while offset != -1:
        sample_rate = read_u32(data, offset + 0x90)[0] if offset + 0x94 <= end else 0
        bits_per_sample = read_u32(data, offset + 0x94)[0] if offset + 0x98 <= end else 0
        channels = read_u32(data, offset + 0x98)[0] if offset + 0x9C <= end else 0
        byte_rate = read_u32(data, offset + 0xE0)[0] if offset + 0xE4 <= end else 0
        video_width = read_u32(data, offset + 0x80)[0] if offset + 0x84 <= end else 0
        video_height = read_u32(data, offset + 0x84)[0] if offset + 0x88 <= end else 0
        video_only = sample_rate == 0 and bits_per_sample == 0 and channels == 0 and byte_rate == 0
        if (
            offset + 0x4E8 <= end
            and read_u32(data, offset + 0x68)[0] == 0xADA77777
            and 0 < video_width <= 2000
            and 0 < video_height <= 2000
            and (
                video_only
                or (
                    sample_rate in (8000, 11025, 22050)
                    and bits_per_sample in (8, 16)
                    and channels in (1, 2)
                )
            )
        ):
            markers.append(offset)
        offset = data.find(b"Board Video File", offset + 1, end)
    return markers


def decode_board_video_rect(
    frame: bytearray,
    width: int,
    height: int,
    payload: bytes,
    left: int,
    top: int,
    right: int,
    bottom: int,
) -> int:
    x = left
    y = top
    offset = 0
    while y < bottom and offset < len(payload):
        code = payload[offset]
        offset += 1
        if code == 0:
            if offset + 2 > len(payload):
                break
            count = payload[offset]
            value = payload[offset + 1]
            offset += 2
            values = bytes([value]) * count
        elif code == 0xFF:
            if offset + 3 > len(payload):
                break
            count = payload[offset] | (payload[offset + 1] << 8)
            value = payload[offset + 2]
            offset += 3
            values = bytes([value]) * count
        else:
            count = code
            values = payload[offset : offset + count]
            offset += len(values)

        for value in values:
            if y >= bottom:
                break
            if 0 <= x < width and 0 <= y < height:
                frame[y * width + x] = value
            x += 1
            if x >= right:
                x = left
                y += 1
    return offset


def extract_board_video_streams(
    data: bytes,
    private_start: int,
    private_end: int,
    component_name: str,
    embedded_dir: Path,
) -> tuple[list[dict], list[dict]]:
    markers = find_board_video_streams(data, private_start, private_end)
    marker_set = set(markers)
    stream_meta = []
    outputs = []
    for stream_index, marker in enumerate(markers):
        sample_rate = read_u32(data, marker + 0x90)[0]
        bits_per_sample = read_u32(data, marker + 0x94)[0]
        channels = read_u32(data, marker + 0x98)[0]
        byte_rate = read_u32(data, marker + 0xE0)[0]
        video_width = read_u32(data, marker + 0x80)[0]
        video_height = read_u32(data, marker + 0x84)[0]
        header_transparent_index = read_u32(data, marker + 0x74)[0] & 0xFF
        frames_per_second = read_u32(data, marker + 0x9C)[0]
        frame_count = read_u32(data, marker + 0x8C)[0]
        header_record_count = read_u32(data, marker + 0x88)[0]
        record_offset = marker + 0x4E8
        audio_chunks = []
        video_frames = []
        video_frame = (
            bytearray([header_transparent_index]) * (video_width * video_height)
            if video_width > 0 and video_height > 0
            else bytearray()
        )
        video_decode_warnings = []
        record_count = 0
        audio_record_count = 0
        video_record_count = 0
        stop_reason = "end"

        while record_offset + 12 <= private_end:
            size = read_u32(data, record_offset)[0]
            tag = read_u32(data, record_offset + 4)[0]
            if size < 12 or record_offset + size > private_end:
                stop_reason = "invalid_record"
                break
            if tag not in (0xFF000001, 0xFF000002):
                stop_reason = "unknown_tag"
                break

            if tag == 0xFF000002:
                if size < 0x4C:
                    stop_reason = "invalid_audio_record"
                    break
                payload_size = size - 0x4C
                audio_start = record_offset + 0x48
                audio_chunks.append(data[audio_start : audio_start + payload_size])
                audio_record_count += 1
            else:
                if size < 0x4C:
                    stop_reason = "invalid_video_record"
                    break
                left = read_u32(data, record_offset + 0x14)[0]
                top = read_u32(data, record_offset + 0x18)[0]
                right = read_u32(data, record_offset + 0x1C)[0]
                bottom = read_u32(data, record_offset + 0x20)[0]
                unchanged = read_u32(data, record_offset + 0x24)[0]
                if (
                    video_frame
                    and left <= right <= video_width
                    and top <= bottom <= video_height
                    and unchanged == 0
                ):
                    payload = data[record_offset + 0x48 : record_offset + size - 4]
                    consumed = decode_board_video_rect(video_frame, video_width, video_height, payload, left, top, right, bottom)
                    if consumed != len(payload):
                        video_decode_warnings.append(
                            {
                                "record_offset": record_offset,
                                "payload_size": len(payload),
                                "consumed": consumed,
                            }
                        )
                elif unchanged not in (0, 1):
                    video_decode_warnings.append({"record_offset": record_offset, "unexpected_unchanged_flag": unchanged})
                if video_frame:
                    video_frames.append(bytes(video_frame))
                video_record_count += 1

            record_count += 1
            record_offset += size
            if record_offset in marker_set:
                stop_reason = "next_stream"
                break

        item = {
            "index": stream_index,
            "offset": marker,
            "end_offset": record_offset,
            "sample_rate": sample_rate,
            "bits_per_sample": bits_per_sample,
            "channels": channels,
            "byte_rate": byte_rate,
            "video_width": video_width,
            "video_height": video_height,
            "header_transparent_index": header_transparent_index,
            "frames_per_second": frames_per_second,
            "frame_count": frame_count,
            "header_record_count": header_record_count,
            "record_count": record_count,
            "audio_record_count": audio_record_count,
            "video_record_count": video_record_count,
            "stop_reason": stop_reason,
        }
        if video_decode_warnings:
            item["video_decode_warnings"] = video_decode_warnings

        if audio_chunks:
            pcm = b"".join(audio_chunks)
            asset_name = f"{component_name}_{stream_index:03d}_{marker:08x}_board_video_audio.wav"
            asset_path = embedded_dir / asset_name
            write_pcm_wave(asset_path, pcm, sample_rate, bits_per_sample, channels)
            duration = len(pcm) / byte_rate if byte_rate else None
            output = {
                "kind": "board_video_pcm_wave",
                "path": str(asset_path),
                "offset": marker,
                "size": asset_path.stat().st_size,
                "pcm_size": len(pcm),
                "duration_seconds": duration,
                "sample_rate": sample_rate,
                "bits_per_sample": bits_per_sample,
                "channels": channels,
                "audio_record_count": audio_record_count,
            }
            item["audio_output"] = str(asset_path)
            item["audio_pcm_size"] = len(pcm)
            item["duration_seconds"] = duration
            outputs.append(output)

        if len(video_frames) >= 2 and video_width > 0 and video_height > 0:
            delay_cs = 10
            if 0 < frames_per_second <= 60:
                delay_cs = max(1, round(100 / frames_per_second))
            transparent_index = header_transparent_index
            palette = data[marker + 0xEC : marker + 0xEC + 1024]
            video_frames = remap_chroma_key_pixels(video_frames, palette, transparent_index)
            asset_name = f"{component_name}_{stream_index:03d}_{marker:08x}_board_video_{video_width}x{video_height}.gif"
            asset_path = embedded_dir.parent / "animations" / asset_name
            still_path = asset_path.with_name(f"{asset_path.stem}_still.png")
            write_indexed_gif(
                asset_path,
                video_frames,
                video_width,
                video_height,
                palette,
                delay_cs=delay_cs,
                transparent_index=transparent_index,
            )
            write_indexed_png(
                still_path,
                video_frames[0],
                video_width,
                video_height,
                palette,
                transparent_index=transparent_index,
            )
            output = {
                "kind": "board_video_gif",
                "path": str(asset_path),
                "still_path": str(still_path),
                "offset": marker,
                "size": asset_path.stat().st_size,
                "still_size": still_path.stat().st_size if still_path.exists() else None,
                "frame_count": len(video_frames),
                "width": video_width,
                "height": video_height,
                "transparent_index": transparent_index,
                "header_transparent_index": header_transparent_index,
                "delay_centiseconds": delay_cs,
            }
            item["video_output"] = str(asset_path)
            item["still_output"] = str(still_path)
            item["transparent_index"] = transparent_index
            outputs.append(output)

        stream_meta.append(item)

    return stream_meta, outputs


def find_riff_chunks(data: bytes, start: int = 0, end: int | None = None) -> list[dict]:
    end = len(data) if end is None else min(end, len(data))
    chunks = []
    offset = data.find(b"RIFF", start, end)
    while offset != -1:
        size = riff_chunk_size(data, offset)
        if size is not None and offset + size <= end:
            chunks.append(
                {
                    "offset": offset,
                    "size": size,
                    "kind": data[offset + 8 : offset + 12].decode("ascii", "replace").rstrip(),
                    "extension": riff_extension(data, offset),
                }
            )
            offset = data.find(b"RIFF", offset + size, end)
        else:
            offset = data.find(b"RIFF", offset + 4, end)
    return chunks


def copy_direct_assets(src_dir: Path, out_dir: Path) -> list[dict]:
    manifest = []
    direct_dir = out_dir / "direct"
    for path in sorted(src_dir.iterdir()):
        if not path.is_file():
            continue
        ext = path.suffix.upper()
        if ext == ".AVI":
            dest = direct_dir / "video" / path.name
            mkdir(dest.parent)
            shutil.copy2(path, dest)
            manifest.append({"source": str(path), "output": str(dest), "kind": "direct_avi", "size": path.stat().st_size})
        elif ext in (".EXS", ".EXS_"):
            data = path.read_bytes()
            if data.startswith(b"RIFF") and data[8:12] == b"WAVE":
                dest = direct_dir / "audio" / (path.stem + ".wav")
                mkdir(dest.parent)
                shutil.copy2(path, dest)
                manifest.append({"source": str(path), "output": str(dest), "kind": "direct_wave", "size": path.stat().st_size})
            else:
                dest = direct_dir / "unknown_exs" / path.name
                mkdir(dest.parent)
                shutil.copy2(path, dest)
                manifest.append({"source": str(path), "output": str(dest), "kind": "direct_exs_unknown", "size": path.stat().st_size})
    return manifest


def parse_bdf_header(data: bytes) -> dict:
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
    palette_offset = offset
    offset += palette_size
    dib_size, offset = read_u32(data, offset)
    dib_offset = offset
    offset += dib_size
    return {
        "banner": banner,
        "version": version,
        "page_rect": rect,
        "layer_range": [min_layer, max_layer],
        "background_flag": background_flag,
        "background_color_index": background_color_index,
        "palette_offset": palette_offset,
        "palette_size": palette_size,
        "dib_offset": dib_offset,
        "dib_size": dib_size,
        "component_list_offset": offset,
    }


def write_bmp_from_dib(path: Path, dib: bytes) -> None:
    if len(dib) < 40:
        return
    header_size = struct.unpack_from("<I", dib, 0)[0]
    if header_size < 40 or len(dib) < header_size:
        return
    bit_count = struct.unpack_from("<H", dib, 14)[0]
    colors_used = struct.unpack_from("<I", dib, 32)[0]
    palette_entries = colors_used if colors_used else ((1 << bit_count) if bit_count <= 8 else 0)
    pixel_offset = 14 + header_size + palette_entries * 4
    file_size = 14 + len(dib)
    bmp_header = b"BM" + struct.pack("<IHHI", file_size, 0, 0, pixel_offset)
    mkdir(path.parent)
    path.write_bytes(bmp_header + dib)


def flip_indexed_rows(pixels: bytes, width: int, height: int) -> bytes:
    if width <= 0 or height <= 0 or len(pixels) < width * height:
        return pixels
    return b"".join(
        pixels[row * width : (row + 1) * width]
        for row in range(height - 1, -1, -1)
    )


def write_indexed_bmp(path: Path, pixels: bytes, width: int, height: int, palette: bytes) -> None:
    if width <= 0 or height <= 0:
        return
    row_stride = (width + 3) & ~3
    if len(pixels) == width * height and row_stride != width:
        padded = bytearray(row_stride * height)
        for row in range(height):
            src = row * width
            dst = row * row_stride
            padded[dst : dst + width] = pixels[src : src + width]
        pixels = bytes(padded)
    palette = palette[:1024]
    if len(palette) < 1024:
        palette = palette + (b"\x00" * (1024 - len(palette)))
    pixel_offset = 14 + 40 + len(palette)
    file_size = pixel_offset + len(pixels)
    dib = struct.pack(
        "<IiiHHIIiiII",
        40,
        width,
        -height,
        1,
        8,
        0,
        len(pixels),
        0,
        0,
        len(palette) // 4,
        0,
    )
    bmp_header = b"BM" + struct.pack("<IHHI", file_size, 0, 0, pixel_offset)
    mkdir(path.parent)
    path.write_bytes(bmp_header + dib + palette + pixels)


def gif_palette_from_bmp_palette(palette: bytes) -> bytes:
    palette = palette[:1024]
    if len(palette) < 1024:
        palette = palette + (b"\x00" * (1024 - len(palette)))
    rgb = bytearray()
    for offset in range(0, 1024, 4):
        blue, green, red, _reserved = palette[offset : offset + 4]
        rgb.extend((red, green, blue))
    return bytes(rgb)


def bright_green_palette_indices(palette: bytes) -> set[int]:
    indices = set()
    palette = palette[:1024]
    for index, offset in enumerate(range(0, len(palette) - 3, 4)):
        blue, green, red, _reserved = palette[offset : offset + 4]
        if red <= 24 and green >= 224 and 24 <= blue <= 96:
            indices.add(index)
    return indices


def remap_chroma_key_pixels(frames: list[bytes], palette: bytes, transparent_index: int) -> list[bytes]:
    chroma_indices = bright_green_palette_indices(palette)
    chroma_indices.discard(transparent_index)
    if not chroma_indices:
        return frames
    remapped = []
    translation = bytes(transparent_index if value in chroma_indices else value for value in range(256))
    for frame in frames:
        remapped.append(frame.translate(translation))
    return remapped


def dominant_border_index(pixels: bytes, width: int, height: int, palette: bytes | None = None) -> int:
    if width <= 0 or height <= 0 or len(pixels) < width * height:
        return 0
    counts: dict[int, int] = {}
    for x in range(width):
        counts[pixels[x]] = counts.get(pixels[x], 0) + 1
        counts[pixels[(height - 1) * width + x]] = counts.get(pixels[(height - 1) * width + x], 0) + 1
    for y in range(1, height - 1):
        counts[pixels[y * width]] = counts.get(pixels[y * width], 0) + 1
        counts[pixels[y * width + width - 1]] = counts.get(pixels[y * width + width - 1], 0) + 1
    if palette is not None:
        chroma = bright_green_palette_indices(palette).intersection(counts)
        if chroma:
            return max(chroma, key=lambda value: counts[value])
    return max(counts, key=counts.get) if counts else 0


def png_chunk(kind: bytes, payload: bytes) -> bytes:
    return struct.pack(">I", len(payload)) + kind + payload + struct.pack(">I", zlib.crc32(kind + payload) & 0xFFFFFFFF)


def write_indexed_png(
    path: Path,
    pixels: bytes,
    width: int,
    height: int,
    palette: bytes,
    transparent_index: int | None = None,
) -> None:
    if width <= 0 or height <= 0 or len(pixels) < width * height:
        return
    mkdir(path.parent)
    scanlines = bytearray()
    for row in range(height):
        start = row * width
        scanlines.append(0)
        scanlines.extend(pixels[start : start + width])
    stream = bytearray(b"\x89PNG\r\n\x1a\n")
    stream.extend(png_chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 3, 0, 0, 0)))
    stream.extend(png_chunk(b"PLTE", gif_palette_from_bmp_palette(palette)))
    if transparent_index is not None and 0 <= transparent_index < 256:
        alpha = bytearray([255] * (transparent_index + 1))
        alpha[transparent_index] = 0
        stream.extend(png_chunk(b"tRNS", bytes(alpha)))
    stream.extend(png_chunk(b"IDAT", zlib.compress(bytes(scanlines), 1)))
    stream.extend(png_chunk(b"IEND", b""))
    path.write_bytes(stream)


def gif_lzw_compress(indexed_pixels: bytes, min_code_size: int = 8) -> bytes:
    """Conservative GIF LZW stream.

    The encoder intentionally emits literal pixel codes and clears the
    dictionary before the code width grows. This is larger than optimal LZW but
    keeps decoder state simple and avoids fragile animations.
    """
    clear_code = 1 << min_code_size
    end_code = clear_code + 1

    code_size = min_code_size + 1
    bit_buffer = 0
    bit_count = 0
    output = bytearray()

    def emit(code: int) -> None:
        nonlocal bit_buffer, bit_count
        bit_buffer |= code << bit_count
        bit_count += code_size
        while bit_count >= 8:
            output.append(bit_buffer & 0xFF)
            bit_buffer >>= 8
            bit_count -= 8

    emit(clear_code)
    codes_since_clear = 0
    for value in indexed_pixels:
        if codes_since_clear >= 200:
            emit(clear_code)
            codes_since_clear = 0
        emit(value)
        codes_since_clear += 1
    emit(end_code)

    if bit_count:
        output.append(bit_buffer & 0xFF)
    return bytes(output)


def gif_data_subblocks(payload: bytes) -> bytes:
    chunks = bytearray()
    for offset in range(0, len(payload), 255):
        chunk = payload[offset : offset + 255]
        chunks.append(len(chunk))
        chunks.extend(chunk)
    chunks.append(0)
    return bytes(chunks)


def write_indexed_gif(
    path: Path,
    frames: list[bytes],
    width: int,
    height: int,
    palette: bytes,
    delay_cs: int = 10,
    transparent_index: int | None = None,
) -> None:
    if not frames or width <= 0 or height <= 0:
        return
    mkdir(path.parent)
    stream = bytearray()
    stream.extend(b"GIF89a")
    stream.extend(struct.pack("<HHBBB", width, height, 0xF7, 0, 0))
    stream.extend(gif_palette_from_bmp_palette(palette))
    stream.extend(b"\x21\xff\x0bNETSCAPE2.0\x03\x01\x00\x00\x00")
    for pixels in frames:
        stream.extend(b"\x21\xf9\x04")
        if transparent_index is None:
            stream.extend(struct.pack("<BHBB", 0, delay_cs, 0, 0))
        else:
            stream.extend(struct.pack("<BHBB", 0x09, delay_cs, transparent_index & 0xFF, 0))
        stream.extend(b"\x2c")
        stream.extend(struct.pack("<HHHHB", 0, 0, width, height, 0))
        stream.append(8)
        stream.extend(gif_data_subblocks(gif_lzw_compress(pixels, 8)))
    stream.extend(b";")
    path.write_bytes(stream)


def extract_multibitmap_frames(
    data: bytes,
    private_start: int,
    private_end: int,
    component_name: str,
    page_dir: Path,
    palette: bytes,
) -> tuple[dict, list[dict]]:
    if private_start + 8 > private_end:
        return {}, []
    version = read_u32(data, private_start)[0]
    declared_count = read_u32(data, private_start + 4)[0]
    offset = private_start + 8
    frames = []
    outputs = []
    gif_candidates = []
    frame_dir = page_dir / "multibitmap"

    for index in range(declared_count):
        if offset + 4 > private_end:
            break
        pixel_size = read_u32(data, offset)[0]
        pixel_offset = offset + 4
        meta_offset = pixel_offset + pixel_size
        if pixel_size <= 0 or meta_offset + 0xC0 > private_end:
            break

        width = read_u32(data, meta_offset + 0x10)[0]
        height = read_u32(data, meta_offset + 0x14)[0]
        frame_name = data[meta_offset + 0x28 : meta_offset + 0x34].split(b"\x00", 1)[0].decode("cp1250", "replace")
        frame_base = f"{component_name}_{index:03d}_{safe_name(frame_name)}"
        frame = {
            "index": index,
            "name": frame_name,
            "pixel_offset": pixel_offset,
            "pixel_size": pixel_size,
            "metadata_offset": meta_offset,
            "width": width,
            "height": height,
        }

        if width > 0 and height > 0 and pixel_size == width * height:
            bmp_path = frame_dir / f"{frame_base}.bmp"
            pixels = flip_indexed_rows(data[pixel_offset : pixel_offset + pixel_size], width, height)
            write_indexed_bmp(bmp_path, pixels, width, height, palette)
            output = {
                "kind": "multibitmap_bmp",
                "path": str(bmp_path),
                "offset": pixel_offset,
                "size": bmp_path.stat().st_size,
                "pixel_size": pixel_size,
                "width": width,
                "height": height,
                "name": frame_name,
            }
            frame["bmp_output"] = str(bmp_path)
            outputs.append(output)
            gif_candidates.append({"width": width, "height": height, "pixels": pixels, "name": frame_name})
        else:
            raw_path = frame_dir / f"{frame_base}.raw"
            mkdir(raw_path.parent)
            raw_path.write_bytes(data[pixel_offset : pixel_offset + pixel_size])
            output = {
                "kind": "multibitmap_raw",
                "path": str(raw_path),
                "offset": pixel_offset,
                "size": raw_path.stat().st_size,
                "pixel_size": pixel_size,
                "width": width,
                "height": height,
                "name": frame_name,
            }
            frame["raw_output"] = str(raw_path)
            outputs.append(output)

        frames.append(frame)
        offset = meta_offset + 0xC0

    grouped_frames = []
    for frame in gif_candidates:
        if not grouped_frames:
            grouped_frames.append(frame)
            continue
        previous = grouped_frames[-1]
        if frame["width"] == previous["width"] and frame["height"] == previous["height"]:
            grouped_frames.append(frame)
            continue
        if len(grouped_frames) >= 2:
            break
        grouped_frames = [frame]

    if len(grouped_frames) >= 2:
        width = grouped_frames[0]["width"]
        height = grouped_frames[0]["height"]
        first_name = safe_name(grouped_frames[0]["name"])
        gif_path = page_dir / "animations" / f"{component_name}_{first_name}_{width}x{height}.gif"
        write_indexed_gif(gif_path, [frame["pixels"] for frame in grouped_frames], width, height, palette)
        outputs.append(
            {
                "kind": "multibitmap_gif",
                "path": str(gif_path),
                "size": gif_path.stat().st_size,
                "frame_count": len(grouped_frames),
                "width": width,
                "height": height,
                "delay_centiseconds": 10,
            }
        )

    return {"version": version, "declared_count": declared_count, "parsed_count": len(frames), "frames": frames}, outputs


def extract_panorama_images(
    data: bytes,
    private_start: int,
    private_end: int,
    component_name: str,
    page_dir: Path,
    palette: bytes,
) -> tuple[dict, list[dict]]:
    if private_start + 8 > private_end:
        return {}, []
    version = read_u32(data, private_start)[0]
    declared_count = read_u32(data, private_start + 4)[0]
    if version != 1 or declared_count <= 0 or declared_count > 32:
        return {}, []

    offset = private_start + 8
    scenes = []
    outputs = []
    panorama_dir = page_dir / "panorama"
    for index in range(declared_count):
        if offset + 0x224 > private_end:
            break
        record_offset = offset
        width = read_u32(data, record_offset + 0x04)[0]
        height = read_u32(data, record_offset + 0x08)[0]
        subimage_count = read_u32(data, record_offset + 0x150)[0]
        region_count = read_u32(data, record_offset + 0x21C)[0]
        pixel_offset = record_offset + 0x224
        pixel_size = width * height
        if not (0 < width <= 10000 and 0 < height <= 10000 and pixel_offset + pixel_size <= private_end):
            break

        bmp_path = panorama_dir / f"{component_name}_{index:03d}_panorama_{width}x{height}.bmp"
        pixels = flip_indexed_rows(data[pixel_offset : pixel_offset + pixel_size], width, height)
        write_indexed_bmp(bmp_path, pixels, width, height, palette)
        output = {
            "kind": "panorama_bmp",
            "path": str(bmp_path),
            "offset": pixel_offset,
            "size": bmp_path.stat().st_size,
            "pixel_size": pixel_size,
            "width": width,
            "height": height,
            "rect": {"x": 0, "y": 0, "width": width, "height": height},
            "geometry_confidence": "serialized_entry",
            "initially_visible": False,
        }
        outputs.append(output)

        offset = pixel_offset + pixel_size
        parsed_subimages = 0
        for _ in range(subimage_count):
            if offset + 0x78 > private_end:
                break
            layer_width = read_u32(data, offset + 0x24)[0]
            layer_height = read_u32(data, offset + 0x2C)[0]
            layer_size = layer_width * layer_height
            if layer_width > 10000 or layer_height > 10000 or offset + 0x78 + layer_size > private_end:
                break
            offset += 0x78 + layer_size
            parsed_subimages += 1
        if parsed_subimages != subimage_count or offset + region_count * 0x34 > private_end:
            break
        offset += region_count * 0x34
        scenes.append(
            {
                "index": index,
                "record_offset": record_offset,
                "pixel_offset": pixel_offset,
                "width": width,
                "height": height,
                "subimage_count": subimage_count,
                "region_count": region_count,
                "bmp_output": str(bmp_path),
            }
        )

    return {"version": version, "declared_count": declared_count, "parsed_count": len(scenes), "scenes": scenes}, outputs


def extract_panorama_holder_images(
    data: bytes,
    private_start: int,
    private_end: int,
    component_name: str,
    page_dir: Path,
) -> tuple[dict, list[dict]]:
    if private_start + 8 > private_end:
        return {}, []
    version = read_u32(data, private_start)[0]
    declared_count = read_u32(data, private_start + 4)[0]
    if version != 0 or declared_count <= 0 or declared_count > 32:
        return {}, []

    offset = private_start + 8
    scenes = []
    outputs = []
    panorama_dir = page_dir / "panorama_holder"
    for index in range(declared_count):
        if offset + 0x228 > private_end:
            break
        record_offset = offset
        dib_size = read_u32(data, record_offset + 0x224)[0]
        dib_offset = record_offset + 0x228
        dib_end = dib_offset + dib_size
        if dib_size < 40 or dib_end > private_end:
            break
        width = abs(read_i32(data, dib_offset + 4)[0])
        height = abs(read_i32(data, dib_offset + 8)[0])
        if not (0 < width <= 10000 and 0 < height <= 10000):
            break

        bmp_path = panorama_dir / f"{component_name}_{index:03d}_panorama_{width}x{height}.bmp"
        write_bmp_from_dib(bmp_path, data[dib_offset:dib_end])
        output = {
            "kind": "panorama_holder_bmp",
            "path": str(bmp_path),
            "offset": dib_offset,
            "size": bmp_path.stat().st_size,
            "dib_size": dib_size,
            "width": width,
            "height": height,
            "rect": {"x": 0, "y": 0, "width": width, "height": height},
            "geometry_confidence": "serialized_entry",
            "initially_visible": False,
        }
        outputs.append(output)

        offset = dib_end
        if offset + 4 > private_end:
            break
        subimage_count = read_u32(data, offset)[0]
        offset += 4
        parsed_subimages = 0
        for _ in range(subimage_count):
            if offset + 4 > private_end:
                break
            block_size = read_u32(data, offset)[0]
            if offset + 4 + block_size > private_end:
                break
            offset += 4 + block_size
            parsed_subimages += 1
        if parsed_subimages != subimage_count or offset + 4 > private_end:
            break
        region_count = read_u32(data, offset)[0]
        offset += 4
        if offset + region_count * 0x48 > private_end:
            break
        offset += region_count * 0x48
        scenes.append(
            {
                "index": index,
                "record_offset": record_offset,
                "dib_offset": dib_offset,
                "dib_size": dib_size,
                "width": width,
                "height": height,
                "subimage_count": subimage_count,
                "region_count": region_count,
                "bmp_output": str(bmp_path),
            }
        )

    return {"version": version, "declared_count": declared_count, "parsed_count": len(scenes), "scenes": scenes}, outputs


def extract_sprite_holder_images(
    data: bytes,
    private_start: int,
    private_end: int,
    component_name: str,
    page_dir: Path,
    palette: bytes,
) -> tuple[dict, list[dict]]:
    if private_start + 12 > private_end:
        return {}, []
    version = read_u32(data, private_start)[0]
    definition_count = read_u32(data, private_start + 4)[0]
    instance_count = read_u32(data, private_start + 8)[0]
    if version != 1 or definition_count < 0 or definition_count > 500 or instance_count < 0 or instance_count > 1000:
        return {}, []
    offset = private_start + 12
    definitions = []
    outputs = []
    sprite_dir = page_dir / "sprite_holder"

    def infer_phase_count(blob_size: int, width: int, height: int) -> int:
        pixel_unit = width * height
        if pixel_unit <= 0:
            return 0
        max_count = min(64, blob_size // pixel_unit)
        best_count = 0
        best_remainder = blob_size
        for count in range(max_count, 0, -1):
            remainder = blob_size - pixel_unit * count
            if 0x80 <= remainder <= 0x1200:
                return count
            if 0 <= remainder < best_remainder:
                best_count = count
                best_remainder = remainder
        return best_count

    for index in range(definition_count):
        if offset + 4 > private_end:
            break
        blob_size = read_u32(data, offset)[0]
        blob_start = offset + 4
        blob_end = blob_start + blob_size
        if blob_size <= 0 or blob_end > private_end:
            break

        name = data[blob_start + 0x04 : blob_start + 0x24].split(b"\x00", 1)[0].decode("cp1250", "replace")
        width = read_u32(data, blob_start + 0x80)[0] if blob_start + 0x84 <= blob_end else 0
        height = read_u32(data, blob_start + 0x84)[0] if blob_start + 0x88 <= blob_end else 0
        phase_hint = read_u32(data, blob_start + 0xB8)[0] if blob_start + 0xBC <= blob_end else 0
        phase_count = infer_phase_count(blob_size, width, height)
        if phase_count <= 0 and 0 < phase_hint <= 64:
            phase_count = phase_hint
        pixel_size = width * height * phase_count
        pixel_offset = blob_end - pixel_size

        definition = {
            "index": index,
            "name": name,
            "record_offset": offset,
            "blob_offset": blob_start,
            "blob_size": blob_size,
            "width": width,
            "height": height,
            "phase_count": phase_count,
            "phase_hint": phase_hint,
        }

        if 0 < width <= 2000 and 0 < height <= 2000 and pixel_offset >= blob_start and pixel_offset + pixel_size <= blob_end:
            phase_outputs = []
            base = f"{component_name}_{index:03d}_{safe_name(name)}"
            for phase in range(phase_count):
                start = pixel_offset + phase * width * height
                pixels = flip_indexed_rows(data[start : start + width * height], width, height)
                transparent_index = dominant_border_index(pixels, width, height, palette)
                png_path = sprite_dir / f"{base}_phase_{phase:02d}.png"
                write_indexed_png(png_path, pixels, width, height, palette, transparent_index=transparent_index)
                phase_outputs.append(
                    {
                        "phase": phase,
                        "path": str(png_path),
                        "size": png_path.stat().st_size,
                    }
                )
            output = {
                "kind": "sprite_holder_png",
                "path": phase_outputs[0]["path"],
                "offset": pixel_offset,
                "size": Path(phase_outputs[0]["path"]).stat().st_size,
                "pixel_size": pixel_size,
                "width": width,
                "height": height,
                "name": name,
                "id": index,
                "definition_id": index,
                "phase_count": phase_count,
                "phase_outputs": phase_outputs,
            }
            definition["phase_outputs"] = phase_outputs
            outputs.append(output)

        definitions.append(definition)
        offset = blob_end

    instances = []
    instance_record_size = 0x8C
    for index in range(instance_count):
        record_offset = offset + index * instance_record_size
        if record_offset + instance_record_size > private_end:
            break
        definition_id = read_u32(data, record_offset)[0]
        field04 = read_i32(data, record_offset + 0x04)[0]
        x = read_i32(data, record_offset + 0x08)[0]
        y = read_i32(data, record_offset + 0x0C)[0]
        layer = read_i32(data, record_offset + 0x18)[0]
        phase = read_i32(data, record_offset + 0x5C)[0]
        visible = read_i32(data, record_offset + 0x88)[0]
        instances.append(
            {
                "index": index,
                "definition_id": definition_id,
                "field04": field04,
                "x": x,
                "y": y,
                "z": layer,
                "phase": phase,
                "visible": visible,
                "record_offset": record_offset,
            }
        )

    return {
        "version": version,
        "definition_count": definition_count,
        "instance_count": instance_count,
        "parsed_definitions": len(definitions),
        "parsed_instances": len(instances),
        "definitions": definitions,
        "instances": instances,
    }, outputs


def extract_bitmap_holder_images(
    data: bytes,
    private_start: int,
    private_end: int,
    component_name: str,
    page_dir: Path,
    palette: bytes,
) -> tuple[dict, list[dict]]:
    if private_start + 8 > private_end:
        return {}, []
    version = read_u32(data, private_start)[0]
    declared_count = read_u32(data, private_start + 4)[0]
    if version != 1 or declared_count < 0 or declared_count > 200:
        return {}, []

    offset = private_start + 8
    frames = []
    outputs = []
    bitmap_dir = page_dir / "bitmap_holder"

    for index in range(declared_count):
        if offset + 0x90 > private_end:
            break
        record_size = read_u32(data, offset)[0]
        record_end = offset + 4 + record_size
        if record_size <= 0 or record_end > private_end:
            break

        left = read_i32(data, offset + 0x0C)[0]
        top = read_i32(data, offset + 0x10)[0]
        right = read_i32(data, offset + 0x14)[0]
        bottom = read_i32(data, offset + 0x18)[0]
        width = right - left
        height = bottom - top
        name = data[offset + 0x38 : offset + 0x44].split(b"\x00", 1)[0].decode("cp1250", "replace")
        # `offset` points at the u32 blob-size prefix. The verified click/blit
        # paths address pixels at blob+0x80; the following 0x10 bytes are the
        # trailer after the pixel plane, not part of the header.
        pixel_offset = offset + 4 + 0x80
        row_stride = (width + 3) & ~3
        pixel_size = row_stride * height
        frame = {
            "index": index,
            "name": name,
            "record_offset": offset,
            "record_size": record_size,
            "pixel_offset": pixel_offset,
            "width": width,
            "height": height,
            "right": right,
            "bottom": bottom,
            "rect": {"x": left, "y": top, "width": width, "height": height},
        }

        if 0 < width <= 2000 and 0 < height <= 2000 and pixel_offset + pixel_size <= record_end:
            bmp_path = bitmap_dir / f"{component_name}_{index:03d}_{safe_name(name)}_{width}x{height}.bmp"
            pixels = data[pixel_offset : pixel_offset + pixel_size]
            pixels = flip_indexed_rows(pixels, row_stride, height)
            write_indexed_bmp(bmp_path, pixels, width, height, palette)
            output = {
                "kind": "bitmap_holder_bmp",
                "path": str(bmp_path),
                "offset": pixel_offset,
                "size": bmp_path.stat().st_size,
                "pixel_size": pixel_size,
                "width": width,
                "height": height,
                "name": name,
                "rect": frame["rect"],
                "geometry_confidence": "serialized_entry",
            }
            frame["bmp_output"] = str(bmp_path)
            outputs.append(output)

        frames.append(frame)
        offset = record_end

    return {"version": version, "declared_count": declared_count, "parsed_count": len(frames), "frames": frames}, outputs


def extract_bdf(path: Path, out_dir: Path, include_raw: bool = False) -> dict:
    data = path.read_bytes()
    page_dir = out_dir / "bdf" / path.stem
    mkdir(page_dir)

    header = parse_bdf_header(data)
    component_list_offset = header["component_list_offset"]
    component_list_version, cursor = read_u32(data, component_list_offset)
    component_count, cursor = read_u32(data, cursor)
    component_offsets = find_next_wrappers(data, cursor, component_count)

    metadata = {
        "source": str(path),
        "size": len(data),
        **{k: v for k, v in header.items() if not k.endswith("_offset")},
        "component_list_offset": component_list_offset,
        "component_list_version": component_list_version,
        "component_count": component_count,
        "components": [],
        "outputs": [],
    }

    if header["palette_size"]:
        palette_path = page_dir / "background_palette.bin"
        palette_path.write_bytes(data[header["palette_offset"] : header["palette_offset"] + header["palette_size"]])
        metadata["outputs"].append({"kind": "palette", "path": str(palette_path), "size": header["palette_size"]})

    if header["dib_size"]:
        dib = data[header["dib_offset"] : header["dib_offset"] + header["dib_size"]]
        dib_path = page_dir / "background.dib"
        dib_path.write_bytes(dib)
        bmp_path = page_dir / "background.bmp"
        write_bmp_from_dib(bmp_path, dib)
        metadata["outputs"].append({"kind": "dib", "path": str(dib_path), "size": len(dib)})
        if bmp_path.exists():
            metadata["outputs"].append({"kind": "bmp", "path": str(bmp_path), "size": bmp_path.stat().st_size})

    script = find_script_block(data)
    script_start = script.get("version_offset") if script else None
    if script and "text_offset" in script:
        script_bytes = data[script["text_offset"] : script["text_end"]]
        script_path = page_dir / "script.txt"
        script_path.write_text(script_bytes.decode("cp1250", "replace"), encoding="utf-8")
        metadata["script"] = script
        metadata["outputs"].append({"kind": "script", "path": str(script_path), "size": len(script_bytes)})

    embedded_dir = page_dir / "embedded"
    for index, component_offset in enumerate(component_offsets):
        wrapper = parse_wrapper(data, component_offset)
        private_start = wrapper["private_offset"]
        next_wrapper = component_offsets[index + 1] if index + 1 < len(component_offsets) else script_start
        private_end = next_wrapper if next_wrapper is not None else len(data)
        component_name = f"{index:02d}_{safe_name(wrapper['display_name'])}"
        component_meta = {
            "index": index,
            "offset": component_offset,
            "clsid": wrapper["clsid"],
            "display_name": wrapper["display_name"],
            "private_offset": private_start,
            "private_size": max(0, private_end - private_start),
            "embedded": [],
        }
        for riff_index, chunk in enumerate(find_riff_chunks(data, private_start, private_end)):
            asset_name = f"{component_name}_{riff_index:03d}_{chunk['offset']:08x}{chunk['extension']}"
            asset_path = embedded_dir / asset_name
            mkdir(asset_path.parent)
            asset_path.write_bytes(data[chunk["offset"] : chunk["offset"] + chunk["size"]])
            item = {"kind": "riff_" + chunk["kind"].lower(), "path": str(asset_path), **chunk}
            component_meta["embedded"].append(item)
            metadata["outputs"].append({"kind": item["kind"], "path": str(asset_path), "size": chunk["size"]})

        marker = data.find(b"Board Video File", private_start, private_end)
        if marker != -1:
            component_meta["board_video_marker_offset"] = marker
            streams, board_outputs = extract_board_video_streams(
                data, private_start, private_end, component_name, embedded_dir
            )
            if streams:
                component_meta["board_video_streams"] = streams
                for output in board_outputs:
                    component_meta["embedded"].append(output)
                    metadata["outputs"].append(output)

        if wrapper["display_name"] == "MultiBitmap":
            multibitmap, multibitmap_outputs = extract_multibitmap_frames(
                data,
                private_start,
                private_end,
                component_name,
                page_dir,
                data[header["palette_offset"] : header["palette_offset"] + header["palette_size"]],
            )
            if multibitmap:
                component_meta["multibitmap"] = multibitmap
                for output in multibitmap_outputs:
                    component_meta["embedded"].append(output)
                    metadata["outputs"].append(output)

        if wrapper["display_name"] == "Sprite_Holder":
            sprite_holder, sprite_outputs = extract_sprite_holder_images(
                data,
                private_start,
                private_end,
                component_name,
                page_dir,
                data[header["palette_offset"] : header["palette_offset"] + header["palette_size"]],
            )
            if sprite_holder:
                component_meta["sprite_holder"] = sprite_holder
                for output in sprite_outputs:
                    component_meta["embedded"].append(output)
                    metadata["outputs"].append(output)

        if wrapper["display_name"] == "Bitmap_Holder":
            bitmap_holder, bitmap_outputs = extract_bitmap_holder_images(
                data,
                private_start,
                private_end,
                component_name,
                page_dir,
                data[header["palette_offset"] : header["palette_offset"] + header["palette_size"]],
            )
            if bitmap_holder:
                component_meta["bitmap_holder"] = bitmap_holder
                for output in bitmap_outputs:
                    component_meta["embedded"].append(output)
                    metadata["outputs"].append(output)

        if wrapper["display_name"] == "Panorama":
            panorama, panorama_outputs = extract_panorama_images(
                data,
                private_start,
                private_end,
                component_name,
                page_dir,
                data[header["palette_offset"] : header["palette_offset"] + header["palette_size"]],
            )
            if panorama:
                component_meta["panorama"] = panorama
                for output in panorama_outputs:
                    component_meta["embedded"].append(output)
                    metadata["outputs"].append(output)

        if wrapper["display_name"] == "Panorama_Holder":
            panorama_holder, panorama_holder_outputs = extract_panorama_holder_images(
                data,
                private_start,
                private_end,
                component_name,
                page_dir,
            )
            if panorama_holder:
                component_meta["panorama_holder"] = panorama_holder
                for output in panorama_holder_outputs:
                    component_meta["embedded"].append(output)
                    metadata["outputs"].append(output)

        if include_raw and private_end > private_start:
            raw_path = page_dir / "private_raw" / f"{component_name}.bin"
            mkdir(raw_path.parent)
            raw_path.write_bytes(data[private_start:private_end])
            component_meta["raw_output"] = str(raw_path)

        metadata["components"].append(component_meta)

    write_json(page_dir / "metadata.json", metadata)
    return metadata


def parse_grp(path: Path, out_dir: Path) -> dict:
    data = path.read_bytes()
    offset = 0
    count, offset = read_u32(data, offset)
    group_dir = out_dir / "grp" / path.stem
    mkdir(group_dir)
    metadata = {"source": str(path), "cursor_count": count, "cursors": [], "outputs": []}
    for index in range(count):
        if offset + 0x2C + 4 > len(data):
            break
        rec_offset = offset
        record = data[offset : offset + 0x2C]
        offset += 0x2C
        pixel_size, offset = read_u32(data, offset)
        pixels = data[offset : offset + pixel_size]
        offset += pixel_size
        fields = struct.unpack_from("<7I", record, 0)
        width = fields[2]
        height = fields[3]
        name = record[0x1C:0x28].split(b"\x00", 1)[0].decode("cp1250", "replace")
        raw_path = group_dir / f"cursor_{index:02d}_{safe_name(name)}.raw"
        raw_path.write_bytes(pixels)
        metadata["outputs"].append({"kind": "cursor_raw", "path": str(raw_path), "size": len(pixels)})
        pgm_path = None
        if width > 0 and height > 0 and width * height == pixel_size:
            pgm_path = group_dir / f"cursor_{index:02d}_{safe_name(name)}.pgm"
            pgm_path.write_bytes(f"P5\n{width} {height}\n255\n".encode("ascii") + pixels)
            metadata["outputs"].append({"kind": "cursor_pgm", "path": str(pgm_path), "size": pgm_path.stat().st_size})
        metadata["cursors"].append(
            {
                "index": index,
                "record_offset": rec_offset,
                "name": name,
                "width": width,
                "height": height,
                "pixel_size": pixel_size,
                "raw_output": str(raw_path),
                "pgm_output": None if pgm_path is None else str(pgm_path),
            }
        )
    metadata["component_list_offset"] = offset

    if offset + 8 <= len(data):
        component_list_version, cursor = read_u32(data, offset)
        component_count, cursor = read_u32(data, cursor)
        component_offsets = find_next_wrappers(data, cursor, component_count)
        metadata["component_list_version"] = component_list_version
        metadata["component_count"] = component_count
        metadata["components"] = []

        embedded_dir = group_dir / "embedded"
        for component_index, component_offset in enumerate(component_offsets):
            wrapper = parse_wrapper(data, component_offset)
            private_start = wrapper["private_offset"]
            private_end = (
                component_offsets[component_index + 1]
                if component_index + 1 < len(component_offsets)
                else len(data)
            )
            component_name = f"{component_index:02d}_{safe_name(wrapper['display_name'])}"
            component_meta = {
                "index": component_index,
                "offset": component_offset,
                "clsid": wrapper["clsid"],
                "display_name": wrapper["display_name"],
                "private_offset": private_start,
                "private_size": max(0, private_end - private_start),
                "embedded": [],
            }

            for riff_index, chunk in enumerate(find_riff_chunks(data, private_start, private_end)):
                asset_name = f"{component_name}_{riff_index:03d}_{chunk['offset']:08x}{chunk['extension']}"
                asset_path = embedded_dir / asset_name
                mkdir(asset_path.parent)
                asset_path.write_bytes(data[chunk["offset"] : chunk["offset"] + chunk["size"]])
                item = {"kind": "riff_" + chunk["kind"].lower(), "path": str(asset_path), **chunk}
                component_meta["embedded"].append(item)
                metadata["outputs"].append({"kind": item["kind"], "path": str(asset_path), "size": chunk["size"]})

            metadata["components"].append(component_meta)

    write_json(group_dir / "metadata.json", metadata)
    return metadata


def extract_folder(
    src_dir: Path,
    out_dir: Path,
    include_raw: bool = False,
    limit_bdf: int | None = None,
    skip_direct: bool = False,
) -> dict:
    mkdir(out_dir)
    manifest = {
        "source_dir": str(src_dir),
        "output_dir": str(out_dir),
        "direct_assets": [] if skip_direct else copy_direct_assets(src_dir, out_dir),
        "bdf": [],
        "grp": [],
        "skipped": [],
        "errors": [],
    }

    for index, path in enumerate(sorted(src_dir.glob("*.BDF"))):
        if limit_bdf is not None and index >= limit_bdf:
            break
        with path.open("rb") as handle:
            header = handle.read(16)
        if header != b"YDP Board data f":
            manifest["skipped"].append({"source": str(path), "reason": "not a YDP Board data file"})
            continue
        try:
            try:
                meta = extract_bdf(path, out_dir, include_raw=include_raw)
            except OSError:
                # Large Windows corpus runs have occasionally produced a
                # transient EINVAL while replacing an already-generated asset;
                # the same page succeeds immediately in isolation. Retry the
                # page once, while still surfacing persistent path errors.
                meta = extract_bdf(path, out_dir, include_raw=include_raw)
            manifest["bdf"].append(
                {
                    "source": str(path),
                    "output": str(out_dir / "bdf" / path.stem),
                    "component_count": meta.get("component_count"),
                    "output_count": len(meta.get("outputs", [])),
                }
            )
        except Exception as exc:
            manifest["errors"].append({"source": str(path), "error": repr(exc)})

    for path in sorted(src_dir.glob("*.GRP")):
        try:
            meta = parse_grp(path, out_dir)
            manifest["grp"].append(
                {
                    "source": str(path),
                    "output": str(out_dir / "grp" / path.stem),
                    "cursor_count": meta.get("cursor_count"),
                    "output_count": len(meta.get("outputs", [])),
                }
            )
        except Exception as exc:
            manifest["errors"].append({"source": str(path), "error": repr(exc)})

    write_json(out_dir / "manifest.json", manifest)
    return manifest


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path, help="GraphBoard DATA folder")
    parser.add_argument("-o", "--output", type=Path, default=Path("extracted_assets"))
    parser.add_argument("--include-raw", action="store_true", help="Also dump unknown component-private blobs")
    parser.add_argument("--limit-bdf", type=int, help="Only process the first N BDF files")
    parser.add_argument("--skip-direct", action="store_true", help="Do not copy direct AVI/WAVE assets")
    args = parser.parse_args()
    manifest = extract_folder(
        args.source,
        args.output,
        include_raw=args.include_raw,
        limit_bdf=args.limit_bdf,
        skip_direct=args.skip_direct,
    )
    print(
        json.dumps(
            {
                "output": str(args.output),
                "bdf": len(manifest["bdf"]),
                "grp": len(manifest["grp"]),
                "direct_assets": len(manifest["direct_assets"]),
                "skipped": len(manifest["skipped"]),
                "errors": len(manifest["errors"]),
            },
            indent=2,
        )
    )


if __name__ == "__main__":
    main()
