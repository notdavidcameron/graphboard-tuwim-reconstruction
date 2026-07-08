// Source-style reconstruction of the useful TransparentVideoHolder.dll
// routines recovered through Ghidra. This is not intended to compile against
// MFC as-is; it is a close C++ interpretation of the disassembly with raw
// offsets preserved where the original class layout is still unknown.
//
// Anchor functions:
//   10007af0 TVH_SerializeOrDeserialize
//   10007990 TVH_WriteArchiveChunked
//   10007a40 TVH_ReadArchiveChunked
//   1000b370 TVH_EncodeRleRect
//   1000b8a0 TVH_DrawRleRect
//   1000b670 TVH_DrawRleRectClipped

#include <stddef.h>
#include <stdint.h>

typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef int32_t  BOOL;
typedef void    *HGLOBAL;
typedef void    *LPVOID;

class CArchive;
class CFile;
class CString;

extern "C" {
void *operator_new(size_t byteCount);
void  operator_delete(void *ptr);

HGLOBAL GlobalAlloc(uint32_t flags, size_t byteCount);
LPVOID  GlobalLock(HGLOBAL handle);
HGLOBAL GlobalHandle(const void *lockedPtr);
BOOL    GlobalUnlock(HGLOBAL handle);
HGLOBAL GlobalFree(HGLOBAL handle);
void    AfxMessageBox(const char *text, uint32_t type, uint32_t helpId);
}

// These names describe the MFC calls used by the disassembly. Their exact
// declarations here are only placeholders for the original imports.
struct CArchiveOps {
    static uint32_t GetObjectSchema(CArchive *archive);
    static uint32_t Read(CArchive *archive, void *dst, uint32_t byteCount);
    static void     Write(CArchive *archive, const void *src, uint32_t byteCount);
    static void     Flush(CArchive *archive);
    static void     FillBuffer(CArchive *archive, uint32_t needed);
};

struct CFileOps {
    static CFile   *Construct(CFile *storage);
    static BOOL     Open(CFile *file, const char *path, uint32_t mode, void *exception);
    static LONG     Seek(CFile *file, LONG offset, uint32_t from);
    static DWORD    GetPosition(CFile *file);
    static DWORD    Read(CFile *file, void *dst, uint32_t byteCount);
    static void     Write(CFile *file, const void *src, uint32_t byteCount);
    static void     Flush(CFile *file);
    static void     Close(CFile *file);
    static void     Remove(const char *path);
    static CString *GetFilePath(CFile *file, CString *out);
};

static const uint32_t kTVHPrivateVersion = 3;
static const uint32_t kBoardVideoHeaderSize = 0x4e8;
static const uint32_t kVideoRecordTag = 0xff000001;
static const uint32_t kAudioRecordTag = 0xff000002;
static const uint32_t kRecordHeaderSize = 0x4c;
static const uint32_t kRecordPayloadOffset = 0x48;
static const uint32_t kTransparentDisabled = 1000;

static inline uint32_t DIBStride8(uint32_t width)
{
    return ((width * 8 + 31) >> 3) & ~3u;
}

static inline uint32_t StillFrameByteCount(uint32_t width, uint32_t height)
{
    return DIBStride8(width) * height;
}

#pragma pack(push, 1)

struct TVH_BoardVideoHeader {
    BYTE     unknown_000[0x68];          // starts with "Board Video File Ver:0"
    uint32_t magic_ada77777;             // +0x068
    BYTE     unknown_06c[0x10];
    uint32_t frameDurationMs;            // +0x07c, used as playback tick step
    int32_t  width;                      // +0x080
    int32_t  height;                     // +0x084
    uint32_t chunkRecordCount;           // +0x088, filled on finalize
    uint32_t frameCount;                 // +0x08c, filled on finalize
    uint32_t audioSampleRate;            // +0x090
    uint32_t audioBitsPerSample;         // +0x094
    uint32_t audioChannels;              // +0x098
    uint32_t keyFrameInterval;           // +0x09c, writer forces full frames
    BYTE     unknown_0a0[0x40];
    uint32_t audioBytesPerRecord;        // +0x0e0
    uint32_t maxCompressedVideoRecord;   // +0x0e4
    BYTE     unknown_0e8[0x400];
};

struct TVH_ChunkRecord {
    uint32_t byteSize;                   // +0x00, includes the 0x4c-byte record header
    uint32_t tag;                        // +0x04, kVideoRecordTag or kAudioRecordTag
    uint32_t nextRecordByteSize;         // +0x08, copied into byteSize for the next read
    uint32_t forceFullOrKeyMarker;       // +0x0c, writer-side key/full marker
    uint32_t frameIndex;                 // +0x10
    int32_t  left;                       // +0x14
    int32_t  top;                        // +0x18
    int32_t  right;                      // +0x1c
    int32_t  bottom;                     // +0x20
    int32_t  unchangedFrame;             // +0x24, 1 when there is no changed rect
    BYTE     unknown_028[0x20];
    BYTE     payload[1];                 // +0x48, RLE data or PCM audio
};

struct TVH_Entry {
    TVH_BoardVideoHeader header;         // +0x000
    int32_t currentFrameScratch;         // +0x4e8, reset from initialFrameScratch
    int32_t currentFrameScratch2;        // +0x4ec, reset from initialFrameScratch2
    int32_t playedToEnd;                 // +0x4f0
    BYTE    unknown_4f4[0x0c];
    int32_t initialFrameScratch;         // +0x500
    int32_t initialFrameScratch2;        // +0x504
    int32_t originalX;                   // +0x508
    int32_t originalY;                   // +0x50c
    int32_t originalZ;                   // +0x510
    BYTE    unknown_514[0x38];
    int32_t stageX;                      // +0x54c
    int32_t stageY;                      // +0x550
    int32_t stageZ;                      // +0x554
    int32_t streamStart;                 // +0x558
    int32_t streamEnd;                   // +0x55c
    uint32_t stillFrame;                 // +0x560, x86 pointer, serialized separately
    uint32_t scratchFrame;               // +0x564, x86 pointer, runtime only
};

#pragma pack(pop)

static_assert(offsetof(TVH_BoardVideoHeader, width) == 0x80, "header width offset");
static_assert(offsetof(TVH_BoardVideoHeader, audioBytesPerRecord) == 0xe0, "audio offset");
static_assert(sizeof(TVH_BoardVideoHeader) == 0x4e8, "board-video header size");
static_assert(offsetof(TVH_ChunkRecord, payload) == 0x48, "chunk payload offset");
static_assert(offsetof(TVH_Entry, stageX) == 0x54c, "entry x offset");
static_assert(offsetof(TVH_Entry, streamStart) == 0x558, "entry stream offset");
static_assert(sizeof(TVH_Entry) == 0x568, "serialized holder entry size");

// Holder layout, only for the fields touched by this reconstruction.
struct TVH_HolderPartial {
    BYTE       unknown_000[0x2c];
    uint32_t   archiveSchema;            // +0x02c
    int32_t    playbackRect[4];          // +0x030
    void      *invalidator;              // +0x040, vtbl +0x0c invalidates a rect
    void      *runtimeEnabled;           // +0x048
    CFile     *videoCacheFile;           // +0x04c
    BYTE       playback[0x180];          // +0x054, TVH_Player in the DLL
    uint32_t   entries[100];             // +0x1d4, x86 TVH_Entry* values
    int32_t    entryCount;               // +0x364
};

// The compiler passed some member functions a this+4 adjusted pointer. These
// helpers keep the source below readable without pretending the inheritance
// layout has been fully recovered.
static inline TVH_Entry *HolderEntry(void *holder, int32_t index)
{
    uint32_t raw = *reinterpret_cast<uint32_t *>(
        reinterpret_cast<BYTE *>(holder) + 0x1d4 + index * 4);
    return reinterpret_cast<TVH_Entry *>(static_cast<uintptr_t>(raw));
}

static inline void SetHolderEntry(void *holder, int32_t index, TVH_Entry *entry)
{
    *reinterpret_cast<uint32_t *>(reinterpret_cast<BYTE *>(holder) + 0x1d4 + index * 4) =
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(entry));
}

static inline int32_t &HolderEntryCount(void *holder)
{
    return *reinterpret_cast<int32_t *>(reinterpret_cast<BYTE *>(holder) + 0x364);
}

static inline BYTE *EntryStillFrame(TVH_Entry *entry)
{
    return reinterpret_cast<BYTE *>(static_cast<uintptr_t>(entry->stillFrame));
}

static inline void SetEntryStillFrame(TVH_Entry *entry, BYTE *ptr)
{
    entry->stillFrame = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(ptr));
}

static inline BYTE *EntryScratchFrame(TVH_Entry *entry)
{
    return reinterpret_cast<BYTE *>(static_cast<uintptr_t>(entry->scratchFrame));
}

static inline void SetEntryScratchFrame(TVH_Entry *entry, BYTE *ptr)
{
    entry->scratchFrame = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(ptr));
}

static inline CFile *&HolderVideoCache(void *holder)
{
    return *reinterpret_cast<CFile **>(reinterpret_cast<BYTE *>(holder) + 0x4c);
}

static inline uint32_t &HolderArchiveSchema(void *holder)
{
    return *reinterpret_cast<uint32_t *>(reinterpret_cast<BYTE *>(holder) + 0x2c);
}

static inline bool ArchiveIsLoading(CArchive *archive)
{
    uint32_t flags = *reinterpret_cast<uint32_t *>(reinterpret_cast<BYTE *>(archive) + 0x14);
    return (flags & 1) != 0;             // MFC CArchive::load bit
}

static inline CFile *ArchiveFile(CArchive *archive)
{
    return *reinterpret_cast<CFile **>(reinterpret_cast<BYTE *>(archive) + 0x20);
}

static inline void ArchiveSkip(CArchive *archive, uint32_t byteCount)
{
    CArchiveOps::Flush(archive);
    CFileOps::Seek(ArchiveFile(archive), byteCount, 1);
}

static void FreeExistingEntries(void *holder)
{
    for (int32_t i = 0; i < HolderEntryCount(holder); ++i) {
        TVH_Entry *entry = HolderEntry(holder, i);
        if (entry == 0) {
            continue;
        }
        if (EntryStillFrame(entry) != 0) {
            operator_delete(EntryStillFrame(entry));
        }
        if (EntryScratchFrame(entry) != 0) {
            operator_delete(EntryScratchFrame(entry));
        }
        operator_delete(entry);
        SetHolderEntry(holder, i, 0);
    }
}

// 100073b0
uint32_t TVH_ComputeZRange(void *holder, int32_t *minZ, int32_t *maxZ)
{
    *minZ = 100000;
    *maxZ = 0;

    for (int32_t i = 0; i < HolderEntryCount(holder); ++i) {
        TVH_Entry *entry = HolderEntry(holder, i);
        if (entry->stageZ < *minZ) {
            *minZ = entry->stageZ;
        }
        if (*maxZ < entry->stageZ) {
            *maxZ = entry->stageZ;
        }
    }

    if (*minZ == 100000 && *maxZ == 0) {
        *minZ = -1;
        *maxZ = -1;
    }
    return 0;
}

// 10007990
uint32_t TVH_WriteArchiveChunked(CArchive *archive, CFile *sourceFile, uint32_t byteCount)
{
    HGLOBAL handle = GlobalAlloc(2, 0xffff);
    void *buffer = GlobalLock(handle);
    if (buffer == 0) {
        return 0;
    }

    while (byteCount != 0) {
        uint32_t chunk = byteCount < 0x10000 ? byteCount : 0xffff;
        if (CFileOps::Read(sourceFile, buffer, chunk) != chunk) {
            return 0;
        }
        CArchiveOps::Write(archive, buffer, chunk);
        byteCount -= chunk;
    }

    CArchiveOps::Flush(archive);
    handle = GlobalHandle(buffer);
    GlobalUnlock(handle);
    handle = GlobalHandle(buffer);
    GlobalFree(handle);
    return 1;
}

// 10007a40
uint32_t TVH_ReadArchiveChunked(CFile *destFile, CArchive *archive, uint32_t byteCount)
{
    HGLOBAL handle = GlobalAlloc(2, 0xffff);
    void *buffer = GlobalLock(handle);
    if (buffer == 0) {
        return 0;
    }

    while (byteCount != 0) {
        uint32_t chunk = byteCount < 0x10000 ? byteCount : 0xffff;
        if (CArchiveOps::Read(archive, buffer, chunk) != chunk) {
            return 0;
        }
        CFileOps::Write(destFile, buffer, chunk);
        byteCount -= chunk;
    }

    CFileOps::Flush(destFile);
    handle = GlobalHandle(buffer);
    GlobalUnlock(handle);
    handle = GlobalHandle(buffer);
    GlobalFree(handle);
    return 1;
}

static uint32_t TVH_LoadPrivateState(void *holder, CArchive *archive)
{
    CFile *cache = HolderVideoCache(holder);

    // Schema 0 stores board-video bytes in the archive and mirrors them into a
    // temporary CFile for runtime playback. Newer schema paths keep offsets
    // into the archive file itself and skip the byte stream here.
    if (HolderArchiveSchema(holder) == 0) {
        // The original closes/removes an old temp file if present, then creates
        // a new temp file through GetTempPathA/GetTempFileNameA.
        if (cache == 0) {
            cache = CFileOps::Construct(static_cast<CFile *>(operator_new(0x10)));
            HolderVideoCache(holder) = cache;
        }
    } else {
        if (cache == 0) {
            cache = CFileOps::Construct(static_cast<CFile *>(operator_new(0x10)));
            HolderVideoCache(holder) = cache;
        } else {
            CFileOps::Close(cache);
        }

        // The DLL then tries to reopen the archive backing file. If that fails,
        // it displays a message and falls back to schema 0/temp-file behavior.
    }

    FreeExistingEntries(holder);

    uint32_t version = 0;
    uint32_t count = 0;
    CArchiveOps::Read(archive, &version, sizeof(version));
    CArchiveOps::Read(archive, &count, sizeof(count));
    HolderEntryCount(holder) = static_cast<int32_t>(count);

    for (uint32_t i = 0; i < count; ++i) {
        uint32_t streamByteCount = 0;
        CArchiveOps::Read(archive, &streamByteCount, sizeof(streamByteCount));

        DWORD streamStart = 0;
        DWORD streamEnd = 0;
        if (HolderArchiveSchema(holder) == 0) {
            streamStart = CFileOps::GetPosition(cache);
            if (!TVH_ReadArchiveChunked(cache, archive, streamByteCount)) {
                AfxMessageBox("Load VideoHolder data error", 0, 0);
                return 1;
            }
            streamEnd = CFileOps::GetPosition(cache);
        } else {
            ArchiveSkip(archive, streamByteCount);
        }

        TVH_Entry *entry = static_cast<TVH_Entry *>(operator_new(sizeof(TVH_Entry)));
        SetHolderEntry(holder, i, entry);
        CArchiveOps::Read(archive, entry, sizeof(TVH_Entry));

        if (version < 3) {
            entry->originalZ = entry->stageZ;
        }

        uint32_t stillBytes = StillFrameByteCount(entry->header.width, entry->header.height);
        SetEntryStillFrame(entry, static_cast<BYTE *>(operator_new(stillBytes)));
        if (EntryStillFrame(entry) == 0) {
            AfxMessageBox("End of memory", 0, 0);
            return 1;
        }

        CArchiveOps::Read(archive, EntryStillFrame(entry), stillBytes);
        SetEntryScratchFrame(entry, 0);
        entry->playedToEnd = 0;

        if (HolderArchiveSchema(holder) == 0) {
            entry->streamStart = streamStart;
            entry->streamEnd = streamEnd;
        }

        entry->originalX = entry->stageX;
        entry->originalY = entry->stageY;
    }

    return 0;
}

static uint32_t TVH_SavePrivateState(void *holder, CArchive *archive)
{
    uint32_t version = kTVHPrivateVersion;
    uint32_t count = static_cast<uint32_t>(HolderEntryCount(holder));

    CArchiveOps::Write(archive, &version, sizeof(version));
    CArchiveOps::Write(archive, &count, sizeof(count));

    CFile *cache = HolderVideoCache(holder);

    for (uint32_t i = 0; i < count; ++i) {
        TVH_Entry *entry = HolderEntry(holder, i);
        int32_t oldStart = entry->streamStart;
        int32_t oldEnd = entry->streamEnd;
        int32_t streamByteCount = oldEnd - oldStart;

        CArchiveOps::Write(archive, &streamByteCount, sizeof(streamByteCount));
        CArchiveOps::Flush(archive);
        entry->streamStart = CFileOps::GetPosition(ArchiveFile(archive));

        if (CFileOps::Seek(cache, oldStart, 0) != oldStart ||
            !TVH_WriteArchiveChunked(archive, cache, streamByteCount)) {
            entry->streamStart = oldStart;
            entry->streamEnd = oldEnd;
            AfxMessageBox("Save VideoHolder data error", 0, 0);
            return 1;
        }

        entry->streamEnd = CFileOps::GetPosition(ArchiveFile(archive));
        CArchiveOps::Write(archive, entry, sizeof(TVH_Entry));
        CArchiveOps::Write(archive, EntryStillFrame(entry),
                           StillFrameByteCount(entry->header.width, entry->header.height));

        // Keep the runtime object pointing back at its cache-file coordinates.
        entry->streamStart = oldStart;
        entry->streamEnd = oldEnd;
    }

    return 0;
}

// 10007af0. Ghidra return value convention here is 0 on success, 1 on error.
uint32_t TVH_SerializeOrDeserialize(void *holder, CArchive *archive)
{
    uint32_t schema = CArchiveOps::GetObjectSchema(archive);
    HolderArchiveSchema(holder) = schema;
    *reinterpret_cast<uint32_t *>(reinterpret_cast<BYTE *>(archive) + 0x0c) = schema;

    if (ArchiveIsLoading(archive)) {
        return TVH_LoadPrivateState(holder, archive);
    }
    return TVH_SavePrivateState(holder, archive);
}

// 10007880
uint32_t TVH_ResetVideo(void *holder, uint32_t runtimeEnabled)
{
    *reinterpret_cast<uint32_t *>(reinterpret_cast<BYTE *>(holder) + 0x48) = runtimeEnabled;

    for (int32_t i = 0; i < HolderEntryCount(holder); ++i) {
        TVH_Entry *entry = HolderEntry(holder, i);
        entry->stageZ = entry->originalZ;
        entry->currentFrameScratch = entry->initialFrameScratch;
        entry->currentFrameScratch2 = entry->initialFrameScratch2;
        entry->stageX = entry->originalX;
        entry->stageY = entry->originalY;
        entry->playedToEnd = 0;
        if (EntryScratchFrame(entry) != 0) {
            operator_delete(EntryScratchFrame(entry));
            SetEntryScratchFrame(entry, 0);
        }
    }

    // If runtime playback is disabled and a player is active, the DLL stops it.
    return 0;
}

// 1000b370. Encodes an 8-bpp bottom-up DIB rectangle using the holder RLE.
int TVH_EncodeRleRect(char *out, int bitmapWidth, int bitmapHeight, int bitsBase,
                      int left, int top, int rectWidth, int rectHeight)
{
    char *write = out;
    int x = left;
    int y = top;

    while (y < bitmapHeight && y - top < rectHeight) {
        uint32_t stride = DIBStride8(bitmapWidth);
        int rowOffset = (bitmapHeight - y - 1) * static_cast<int>(stride);
        int negStride = -static_cast<int>(stride);
        char value = *reinterpret_cast<char *>(bitsBase + rowOffset + x);

        int run = 1;
        int scanX = x;
        int scanY = y;
        int scanRowOffset = rowOffset;
        for (;;) {
            ++scanX;
            if (bitmapWidth <= scanX || rectWidth <= scanX - left) {
                ++scanY;
                scanRowOffset += negStride;
                scanX = left;
                if (bitmapHeight <= scanY || rectHeight <= scanY - top) {
                    break;
                }
            }
            if (*reinterpret_cast<char *>(bitsBase + scanRowOffset + scanX) != value ||
                ++run > 0xfff9) {
                break;
            }
        }

        if (run >= 4) {
            if (run < 0xff) {
                *write++ = 0x00;
                *write++ = static_cast<char>(run);
            } else {
                *write++ = static_cast<char>(0xff);
                *reinterpret_cast<int16_t *>(write) = static_cast<int16_t>(run);
                write += 2;
            }
            *write++ = value;
        } else {
            run = 1;
            int litX = x;
            int litY = y;
            int litRowOffset = rowOffset;

            for (;;) {
                ++litX;
                if (bitmapWidth <= litX || rectWidth <= litX - left) {
                    ++litY;
                    litRowOffset += negStride;
                    litX = left;
                    if (bitmapHeight <= litY || rectHeight <= litY - top) {
                        break;
                    }
                }

                int sameAfterThis = 0;
                int probeX = litX;
                int probeY = litY;
                int probeRowOffset = litRowOffset;
                for (;;) {
                    ++probeX;
                    if (bitmapWidth <= probeX || rectWidth <= probeX - left) {
                        ++probeY;
                        probeRowOffset += negStride;
                        probeX = left;
                        if (bitmapHeight <= probeY || rectHeight <= probeY - top) {
                            break;
                        }
                    }
                    if (*reinterpret_cast<char *>(bitsBase + probeRowOffset + probeX) !=
                        *reinterpret_cast<char *>(bitsBase + litRowOffset + litX) ||
                        ++sameAfterThis > 3) {
                        break;
                    }
                }

                if (sameAfterThis > 3 || ++run > 0xfd) {
                    break;
                }
            }

            *write++ = static_cast<char>(run);
            while (run-- > 0) {
                *write++ = *reinterpret_cast<char *>(bitsBase + rowOffset + x);
                ++x;
                if (bitmapWidth <= x || rectWidth <= x - left) {
                    ++y;
                    rowOffset += negStride;
                    x = left;
                    if (bitmapHeight <= y || rectHeight <= y - top) {
                        return static_cast<int>(write - out);
                    }
                    if (rectHeight <= y - top) {
                        break;
                    }
                }
            }
            continue;
        }

        while (run-- > 0) {
            ++x;
            if (bitmapWidth <= x || rectWidth <= x - left) {
                x = left;
                ++y;
                if (bitmapHeight <= y || rectHeight <= y - top) {
                    return static_cast<int>(write - out);
                }
            }
        }
    }

    return static_cast<int>(write - out);
}

static inline void TVH_AdvanceRlePixel(BYTE *&dst, int &remainingInRow,
                                       int &rowsRemaining, int rowAdvance,
                                       int rectWidth)
{
    ++dst;
    if (--remainingInRow == 0) {
        if (--rowsRemaining == 0) {
            return;
        }
        dst -= rowAdvance;
        remainingInRow = rectWidth;
    }
}

// 1000b8a0. Draws an RLE rectangle into an 8-bpp bottom-up DIB. When
// transparentIndex <= 0xff, pixels equal to that index are skipped.
int TVH_DrawRleRect(int dstWidth, int dstHeight, int dstBits,
                    int left, int top, const BYTE *rle,
                    int rectWidth, int rectHeight, uint32_t transparentIndex)
{
    if (dstWidth + left < 1 || dstHeight + top < 1 ||
        dstWidth <= left || dstHeight <= top) {
        return 0;
    }

    if (left < 0 || dstWidth < left + rectWidth ||
        top < 0 || dstHeight < top + rectHeight) {
        extern int TVH_DrawRleRectClipped(int, int, int, int, int, const BYTE *,
                                          int, int, uint32_t);
        return TVH_DrawRleRectClipped(dstWidth, dstHeight, dstBits, left, top,
                                      rle, rectWidth, rectHeight, transparentIndex);
    }

    uint32_t stride = DIBStride8(dstWidth);
    int rowAdvance = rectWidth + static_cast<int>(stride);
    BYTE *dst = reinterpret_cast<BYTE *>(dstBits + (dstHeight - top - 1) * stride + left);
    int remainingInRow = rectWidth;
    int rowsRemaining = rectHeight;
    bool useTransparency = transparentIndex <= 0xff;

    for (;;) {
        BYTE op = *rle++;
        uint32_t count;
        BYTE value;

        if (op == 0x00) {
            count = *rle++;
            value = *rle++;
            while (count-- != 0) {
                if (!useTransparency || value != static_cast<BYTE>(transparentIndex)) {
                    *dst = value;
                }
                TVH_AdvanceRlePixel(dst, remainingInRow, rowsRemaining, rowAdvance, rectWidth);
                if (rowsRemaining == 0) {
                    return 1;
                }
            }
        } else if (op == 0xff) {
            count = *reinterpret_cast<const uint16_t *>(rle);
            rle += 2;
            value = *rle++;
            while (count-- != 0) {
                if (!useTransparency || value != static_cast<BYTE>(transparentIndex)) {
                    *dst = value;
                }
                TVH_AdvanceRlePixel(dst, remainingInRow, rowsRemaining, rowAdvance, rectWidth);
                if (rowsRemaining == 0) {
                    return 1;
                }
            }
        } else {
            count = op;
            while (count-- != 0) {
                value = *rle++;
                if (!useTransparency || value != static_cast<BYTE>(transparentIndex)) {
                    *dst = value;
                }
                TVH_AdvanceRlePixel(dst, remainingInRow, rowsRemaining, rowAdvance, rectWidth);
                if (rowsRemaining == 0) {
                    return 1;
                }
            }
        }
    }
}

// 1000b670. Same decoder as above, but clips writes against the destination.
// Return value is the number of RLE bytes consumed, matching the DLL helper.
int TVH_DrawRleRectClipped(int dstWidth, int dstHeight, int dstBits,
                           int left, int top, const BYTE *rle,
                           int rectWidth, int rectHeight,
                           uint32_t transparentIndex)
{
    uint32_t stride = DIBStride8(dstWidth);
    int x = left;
    int y = top;
    const BYTE *start = rle;
    bool useTransparency = transparentIndex <= 0xff;

    while (y < dstHeight && y - top < rectHeight) {
        BYTE op = *rle++;
        uint32_t count;
        BYTE value;

        if (op == 0x00) {
            count = *rle++;
            value = *rle++;
        } else if (op == 0xff) {
            count = *reinterpret_cast<const uint16_t *>(rle);
            rle += 2;
            value = *rle++;
        } else {
            count = op;
            while (count-- != 0) {
                value = *rle++;
                if ((!useTransparency || value != static_cast<BYTE>(transparentIndex)) &&
                    x >= 0 && y >= 0 && x < static_cast<int>(stride)) {
                    int dstRow = dstHeight - y - 1;
                    *reinterpret_cast<BYTE *>(dstBits + stride * dstRow + x) = value;
                }
                ++x;
                if (rectWidth <= x - left) {
                    ++y;
                    x = left;
                    if (dstHeight <= y || rectHeight <= y - top) {
                        return static_cast<int>(rle - start);
                    }
                }
            }
            continue;
        }

        while (count-- != 0) {
            if ((!useTransparency || value != static_cast<BYTE>(transparentIndex)) &&
                x >= 0 && y >= 0 && x < static_cast<int>(stride)) {
                int dstRow = dstHeight - y - 1;
                *reinterpret_cast<BYTE *>(dstBits + stride * dstRow + x) = value;
            }
            ++x;
            if (rectWidth <= x - left) {
                ++y;
                x = left;
                if (dstHeight <= y || rectHeight <= y - top) {
                    return static_cast<int>(rle - start);
                }
            }
        }
    }

    return static_cast<int>(rle - start);
}

// 1000afe0. The writer compares the incoming frame with its previous frame,
// finds the smallest changed rectangle, then emits either:
//   - byteSize 0x4c with unchangedFrame=1, or
//   - byteSize 0x4c + TVH_EncodeRleRect(...)
// It copies the incoming full frame into the previous-frame buffer afterward.

// 1000a620/1000a880/1000aab0 are the corresponding board-video writer:
//   Begin: write the 0x4e8 header, allocate work buffers, emit initial silent
//          audio records if the header says audio is present.
//   Append: optionally repeats the previous audio record, builds a video record
//           via 1000afe0, tracks maxCompressedVideoRecord at header +0xe4.
//   End: write a final record whose nextRecordByteSize is 0, seek back to the
//        header, and patch chunkRecordCount/frameCount/maxCompressedVideoRecord.
