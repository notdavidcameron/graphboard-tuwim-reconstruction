# GraphBoard Asset Extraction

Source folder: `C:\Users\Administrator\Desktop\Multimedialny świat Juliana Tuwima PL\Tuwim\DATA`

Extractor: `graphboard_extract_assets.py`

## Summary

```text
real BDF pages processed: 201
GRP files processed: 7
direct AVI/WAVE assets copied: 56
skipped files: 1
errors: 0
```

`KOTEK2.BDF` was skipped because it is byte-identical to `DIRS.EXE` and starts with an `MZ` executable header, not a GraphBoard page header.

## Layout

```text
direct/audio/              WAV files copied from .EXS/.EXS_
direct/video/              AVI files copied from .AVI
bdf/<PAGE>/metadata.json   parsed page/component/script metadata
bdf/<PAGE>/script.txt      page script text, when present
bdf/<PAGE>/embedded/       RIFF chunks carved from component payloads
bdf/<PAGE>/background.*    DIB/BMP backgrounds, when present
bdf/<PAGE>/background_palette.bin
grp/<GROUP>/metadata.json  cursor/group metadata
grp/<GROUP>/*.pgm          grayscale cursor previews
grp/<GROUP>/*.raw          original cursor pixel-index bytes
manifest.json              top-level extraction manifest
```

Unknown component-private encodings, such as sprite and multibitmap internals, were not dumped wholesale. Use:

```text
python graphboard_extract_assets.py "<DATA folder>" -o extracted_assets_raw --include-raw
```

to preserve those private payload blobs as well.
