// Source-style reconstruction of the component-private serializers for
// SpriteHolder.dll and HotSpotHolder.dll, recovered through Ghidra. These are
// the per-component blocks the host reaches through the wrapper's interface
// vtable slot +0x44 (see GraphBrdCntrItem_Serialize in
// GraphBoardDocument_reconstructed.cpp). As with the other reconstructions this
// is a close C++ interpretation with raw field offsets preserved; the inlined
// MFC CArchive bookkeeping is expressed through the CArchiveOps helpers.
//
// Anchor functions:
//   SpriteHolder.dll   10008930  SpriteHolder_SerializePrivateState
//   HotSpotHolder.dll  10004d10  HotSpotHolder_SerializePrivateState
//
// Both verified byte-exact against RZECZKA.BDF:
//   sprite  private @0x598572: version 1, 5 defs, 5 instances, ends at 0x641bea
//   hotspot private @0x0008e6: version 0, 8 hotspots, ends at 0x000c21 (827 B)

#include <stdint.h>
#include <stddef.h>

typedef void *HGLOBAL;
typedef void *LPVOID;

class CArchive;
class CString;

extern "C" {
void *operator_new(size_t byteCount);
void  operator_delete(void *ptr);
HGLOBAL GlobalAlloc(uint32_t flags, size_t byteCount);
LPVOID  GlobalLock(HGLOBAL handle);
HGLOBAL GlobalHandle(const void *lockedPtr);
uint32_t GlobalUnlock(HGLOBAL handle);
HGLOBAL GlobalFree(HGLOBAL handle);
size_t  GlobalSize(HGLOBAL handle);
}

struct CArchiveOps {
    static bool     IsStoring(CArchive *ar);
    static uint32_t Read(CArchive *ar, void *dst, uint32_t byteCount);
    static void     Write(CArchive *ar, const void *src, uint32_t byteCount);
    static uint32_t ReadU32(CArchive *ar);
    static void     WriteU32(CArchive *ar, uint32_t v);
    static uint8_t  ReadU8(CArchive *ar);
    static void     WriteU8(CArchive *ar, uint8_t v);
};

struct CStringOps {
    static void Extract(CArchive *ar, CString *dst);
    static void Insert(CArchive *ar, const CString *s);
    static void Construct(CString *s);
    static void Destruct(CString *s);
};

extern void AfxMessageBox(const char *text, uint32_t type, uint32_t helpId);

template <typename T>
static inline T &Field(void *base, size_t off) {
    return *reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(base) + off);
}

// ===========================================================================
// SpriteHolder.dll  10008930  SpriteHolder_SerializePrivateState
//
//   u32 version(=1)
//   u32 definitionCount
//   u32 instanceCount
//   repeat definitionCount:
//       u32  blobByteCount
//       BYTE blob[blobByteCount]        (self-describing CSpriteDef: name at
//                                        +0x0c, width +0x88, height +0x8c,
//                                        phase table of 0x4c-byte entries at
//                                        +0x8c, phase count +0xc0; pixel data
//                                        trails the record — see extractor)
//   repeat instanceCount:
//       BYTE record[0x8c]               (CSpriteParam instance; record+0x00 is
//                                        a definition INDEX on disk, rebound to
//                                        the definition pointer on load)
//
// The definition blobs are the pixel/animation sources; the instance records
// are placed sprites that reference a definition by index.
// ===========================================================================
static const size_t   kOffSpriteDefTable   = 0x2c;   // void*[] of definition blobs
static const size_t   kOffSpriteDefCount   = 0x284;
static const size_t   kOffSpriteInstTable  = 0x288;  // void*[] of instance records
static const size_t   kOffSpriteInstCount  = 0x4e0;
static const size_t   kSpriteInstanceSize  = 0x8c;
static const size_t   kOffInstanceDefRef   = 0x00;   // def index (disk) / def ptr (mem)
static const size_t   kOffInstanceDefCopy  = 0x18;   // mirror of record+0x04 on load
static const uint32_t kSpriteVersion       = 1;

void SpriteHolder_SerializePrivateState(void *holder, CArchive *ar)
{
    void **defTable  = &Field<void *>(holder, kOffSpriteDefTable);
    void **instTable = &Field<void *>(holder, kOffSpriteInstTable);
    int   &defCount  = Field<int>(holder, kOffSpriteDefCount);
    int   &instCount = Field<int>(holder, kOffSpriteInstCount);

    if (!CArchiveOps::IsStoring(ar)) {
        // Release the previous definition blobs and instance records.
        for (int i = 0; i < defCount; ++i) {
            GlobalUnlock(GlobalHandle(defTable[i]));
            GlobalFree(GlobalHandle(defTable[i]));
        }
        for (int i = 0; i < instCount; ++i) {
            operator_delete(instTable[i]);
        }

        (void)CArchiveOps::ReadU32(ar);              // version, ignored
        defCount  = static_cast<int>(CArchiveOps::ReadU32(ar));
        instCount = static_cast<int>(CArchiveOps::ReadU32(ar));

        for (int i = 0; i < defCount; ++i) {
            uint32_t blobBytes = CArchiveOps::ReadU32(ar);
            LPVOID blob = GlobalLock(GlobalAlloc(0x42, blobBytes));
            defTable[i] = blob;
            if (blob == nullptr) {
                AfxMessageBox("Sprite load error - End of memory", 0, 0);
                return;
            }
            CArchiveOps::Read(ar, blob, blobBytes);
        }

        for (int i = 0; i < instCount; ++i) {
            void *rec = operator_new(kSpriteInstanceSize);
            instTable[i] = rec;
            CArchiveOps::Read(ar, rec, kSpriteInstanceSize);
            // Rebind the stored definition index to the loaded definition ptr.
            int defIndex = Field<int>(rec, kOffInstanceDefRef);
            Field<void *>(rec, kOffInstanceDefRef) = defTable[defIndex];
            Field<uint32_t>(rec, kOffInstanceDefCopy) = Field<uint32_t>(rec, 0x04);
        }
    } else {
        CArchiveOps::WriteU32(ar, kSpriteVersion);
        CArchiveOps::WriteU32(ar, static_cast<uint32_t>(defCount));
        CArchiveOps::WriteU32(ar, static_cast<uint32_t>(instCount));

        for (int i = 0; i < defCount; ++i) {
            uint32_t blobBytes = static_cast<uint32_t>(GlobalSize(GlobalHandle(defTable[i])));
            CArchiveOps::WriteU32(ar, blobBytes);
            CArchiveOps::Write(ar, defTable[i], blobBytes);
        }

        for (int i = 0; i < instCount; ++i) {
            void *rec = instTable[i];
            void *defPtr = Field<void *>(rec, kOffInstanceDefRef);
            // Convert the definition pointer back to its table index for disk.
            for (int j = 0; j < defCount; ++j) {
                if (defTable[j] == defPtr) {
                    Field<int>(rec, kOffInstanceDefRef) = j;
                    CArchiveOps::Write(ar, rec, kSpriteInstanceSize);
                    Field<void *>(rec, kOffInstanceDefRef) = defPtr;
                    break;
                }
            }
        }
    }
}

// ===========================================================================
// HotSpotHolder.dll  10004d10  HotSpotHolder_SerializePrivateState
//
//   u32 version(=0)
//   u8  flag0 (holder+0x1d0)            (RZECZKA: 210)
//   u8  flag1 (holder+0x1d1)            (RZECZKA: 0)
//   u8  flag2 (holder+0x1d2)            (RZECZKA: 255)
//   u32 hotspotCount (holder+0x1c4)
//   repeat hotspotCount:
//       BYTE record[100]               (CHotSpot: rect left/top/right/bottom at
//                                        +0x00/+0x04/+0x08/+0x0c, layer at +0x1c,
//                                        enabled flag at +0x20; +0x10 name ptr is
//                                        stale on disk and rebuilt below)
//       CString name
//   u32 field1f0 (holder+0x1f0)         (RZECZKA: 8 — active/selected index)
//   u32 field1c8 (holder+0x1c8)
//
// The 100-byte record carries the real hit-test rectangle and layer used by
// HotSpotHolder_TestPointInEnabledHotSpot (10004*): a point hits when
// left <= x <= right, top <= y < bottom, the hotspot is enabled (+0x20 != 0)
// and its layer (+0x1c) matches the query layer.
// ===========================================================================
struct HotSpotRecord {                   // operator_new(100 == 0x64)
    int32_t  left;                       // +0x00
    int32_t  top;                        // +0x04
    int32_t  right;                      // +0x08
    int32_t  bottom;                     // +0x0c
    CString *name;                       // +0x10 (stale on disk)
    // ... remaining fields: +0x1c layer, +0x20 enabled, runtime state ...
};

static const size_t   kOffHotSpotTable   = 0x34;    // void*[] of records
static const size_t   kOffHotSpotCount   = 0x1c4;
static const size_t   kOffHotSpotFlag0   = 0x1d0;
static const size_t   kOffHotSpotFlag1   = 0x1d1;
static const size_t   kOffHotSpotFlag2   = 0x1d2;
static const size_t   kOffHotSpotField1c8 = 0x1c8;
static const size_t   kOffHotSpotField1f0 = 0x1f0;
static const size_t   kOffHotSpotName    = 0x10;    // within a record
static const size_t   kHotSpotRecordSize = 100;
static const uint32_t kHotSpotVersion    = 0;

void HotSpotHolder_SerializePrivateState(void *holder, CArchive *ar)
{
    void **table = &Field<void *>(holder, kOffHotSpotTable);
    int   &count = Field<int>(holder, kOffHotSpotCount);

    if (!CArchiveOps::IsStoring(ar)) {
        // Release the previous hotspot records and their name strings.
        for (int i = 0; i < count; ++i) {
            void *rec = table[i];
            if (rec) {
                CString *name = Field<CString *>(rec, kOffHotSpotName);
                if (name) { CStringOps::Destruct(name); operator_delete(name); }
                operator_delete(rec);
            }
        }

        (void)CArchiveOps::ReadU32(ar);              // version, ignored
        Field<uint8_t>(holder, kOffHotSpotFlag0) = CArchiveOps::ReadU8(ar);
        Field<uint8_t>(holder, kOffHotSpotFlag1) = CArchiveOps::ReadU8(ar);
        Field<uint8_t>(holder, kOffHotSpotFlag2) = CArchiveOps::ReadU8(ar);
        count = static_cast<int>(CArchiveOps::ReadU32(ar));

        for (int i = 0; i < count; ++i) {
            void *rec = operator_new(kHotSpotRecordSize);
            table[i] = rec;
            CArchiveOps::Read(ar, rec, kHotSpotRecordSize);
            CString *name = static_cast<CString *>(operator_new(4));
            CStringOps::Construct(name);
            Field<CString *>(rec, kOffHotSpotName) = name;
            CStringOps::Extract(ar, name);
        }

        Field<uint32_t>(holder, kOffHotSpotField1f0) = CArchiveOps::ReadU32(ar);
        Field<uint32_t>(holder, kOffHotSpotField1c8) = CArchiveOps::ReadU32(ar);
        Field<uint32_t>(holder, 0x1f4) = 0;          // runtime flag cleared on load
    } else {
        CArchiveOps::WriteU32(ar, kHotSpotVersion);
        CArchiveOps::WriteU8(ar, Field<uint8_t>(holder, kOffHotSpotFlag0));
        CArchiveOps::WriteU8(ar, Field<uint8_t>(holder, kOffHotSpotFlag1));
        CArchiveOps::WriteU8(ar, Field<uint8_t>(holder, kOffHotSpotFlag2));
        CArchiveOps::WriteU32(ar, static_cast<uint32_t>(count));

        for (int i = 0; i < count; ++i) {
            void *rec = table[i];
            CArchiveOps::Write(ar, rec, kHotSpotRecordSize);
            CStringOps::Insert(ar, Field<CString *>(rec, kOffHotSpotName));
        }

        CArchiveOps::WriteU32(ar, Field<uint32_t>(holder, kOffHotSpotField1f0));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(holder, kOffHotSpotField1c8));
    }
}
