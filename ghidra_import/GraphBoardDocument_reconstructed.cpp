// Source-style reconstruction of the Tuwim.exe host-document serializers
// recovered through Ghidra. Like TransparentVideoHolder_reconstructed.cpp this
// is not meant to compile against MFC as-is; it is a close C++ interpretation
// of the disassembly with raw CGraphBrdDoc field offsets preserved where the
// full class layout is still unknown. The inlined MFC CArchive insert/extract
// bookkeeping (FillBuffer/Flush + m_lpBufCur juggling) is expressed through the
// CArchiveOps helpers below; the on-disk byte order is identical to the binary.
//
// Anchor functions (Tuwim.exe):
//   00406020 GraphBrdDoc_SerializeProjectState        (START.PRJ)
//   004066f0 GraphBrdDoc_SerializePageData            (.BDF page files)
//   004064c0 GraphBrdDoc_SerializeCursorAndGroupState (.GRP group files)
//   004230a0 GraphBrdScriptEditor_SerializeText       (script text block)
//   0042b920 GraphBrdCntrItem_Serialize               (component wrapper records)
//   0041aad0 GraphBrdScriptEngine_Serialize           (parsed-script index state)
//
// Cross-checked byte-exact against START.PRJ (827 bytes), RZECZKA.BDF, and
// CURSORS.GRP. See graphboard_file_formats.md for the narrative format notes.

#include <stdint.h>
#include <stddef.h>

typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef int32_t  BOOL;
typedef void    *HGLOBAL;
typedef void    *LPVOID;

class CArchive;          // MFC serialization stream
class CString;
class CGraphBrdDoc;      // COleDocument-derived host document
class CScriptEditorDlg;  // rich-edit script editor dialog

extern "C" {
void *operator_new(size_t byteCount);
void  operator_delete(void *ptr);
HGLOBAL GlobalAlloc(uint32_t flags, size_t byteCount);
LPVOID  GlobalLock(HGLOBAL handle);
HGLOBAL GlobalHandle(const void *lockedPtr);
BOOL    GlobalUnlock(HGLOBAL handle);
HGLOBAL GlobalFree(HGLOBAL handle);
size_t  GlobalSize(HGLOBAL handle);
}

// ---------------------------------------------------------------------------
// MFC primitive helpers. The real code inlines these; the reconstruction keeps
// the on-disk order but drops the buffer-pointer arithmetic for readability.
// ---------------------------------------------------------------------------
struct CArchiveOps {
    static bool     IsStoring(CArchive *ar);              // (ar->m_nMode & 1) != 0 stores
    static uint32_t Read(CArchive *ar, void *dst, uint32_t byteCount);
    static void     Write(CArchive *ar, const void *src, uint32_t byteCount);
    static uint32_t ReadU32(CArchive *ar);               // ar >> DWORD
    static void     WriteU32(CArchive *ar, uint32_t v);  // ar << DWORD
    static uint8_t  ReadU8(CArchive *ar);                // ar >> BYTE
    static void     WriteU8(CArchive *ar, uint8_t v);    // ar << BYTE
};

// MFC CString archive I/O. On disk: length prefix (u8, or 0xff+u16, or
// 0xff 0xffff+u32) then raw bytes, no trailing NUL.
struct CStringOps {
    static void Extract(CArchive *ar, CString *dst);     // ar >> *dst
    static void Insert(CArchive *ar, const CString *s);  // ar << *s
    static int  GetLength(const CString *s);
    static char GetAt(const CString *s, int index);
    static void SetAt(CString *s, int index, char ch);
    static void Empty(CString *s);
    static void Construct(CString *s);
    static void Destruct(CString *s);
};

// ---------------------------------------------------------------------------
// CGraphBrdDoc field accessors. Offsets are the raw document-object offsets
// observed in the disassembly (GraphBrdDoc_Construct @ 00405780). Only the
// fields touched by the serializers are named here.
// ---------------------------------------------------------------------------
template <typename T>
static inline T &Field(void *base, size_t off) {
    return *reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(base) + off);
}

// Owned helper objects (each is a serializable dialog/manager whose vtable
// slot +0x08 is CObject::Serialize).
static const size_t kOffPageScriptEditor   = 0x9c;  // per-page script text dialog
static const size_t kOffGroupScriptEditor  = 0xa0;  // group / global script text dialog
static const size_t kOffPageComponentList  = 0xa4;  // page component-list dialog
static const size_t kOffGroupComponentList = 0xa8;  // group component-list dialog
static const size_t kOffScriptEngine       = 0xb4;  // GraphBoard script engine
static const size_t kOffAudioManager       = 0xbc;  // audio manager
static const size_t kOffPageDibPacked       = 0xfc; // HGLOBAL of packed DIB
static const size_t kOffScriptParsedFlag    = 0xe4;

// Page geometry / background (written by SerializePageData, in disk order).
static const size_t kOffPageMinLayer       = 0x100;
static const size_t kOffPageMaxLayer       = 0x104;
static const size_t kOffPageLeft           = 0x108;
static const size_t kOffPageTop            = 0x10c;
static const size_t kOffPageRight          = 0x110;
static const size_t kOffPageBottom         = 0x114;
static const size_t kOffPageBackgroundFlag = 0x118;
static const size_t kOffPageBgColorIndex   = 0x11c;  // one byte

// Cursor table (SerializeCursorAndGroupState). Inline array of up to 50
// pointers, each to a 0x2c record whose +0x28 field is a GlobalAlloc pixel buf.
static const size_t kOffCursorTable        = 0x124;
static const size_t kOffCursorCount        = 0x1ec;
static const size_t kCursorRecordSize      = 0x2c;
static const size_t kOffCursorPixels       = 0x28;   // within a cursor record

// Project manifest (SerializeProjectState).
static const size_t kOffStartupPage        = 0x1f4;  // CString
static const size_t kOffCurrentState       = 0x1f8;  // CString
static const size_t kOffSignature          = 0x1fc;  // CString "Julian Tuwim"
static const size_t kOffPageNamesPtr       = 0x200;  // CString*[]
static const size_t kOffPageCount          = 0x204;
static const size_t kOffGroupNamesPtr      = 0x208;  // CString*[]
static const size_t kOffGroupCount         = 0x20c;
static const size_t kOffAudioPresetSlot    = 0x20;   // within audio manager object

// Script editor dialog: serialized buffer lives at editor+0xe0.
static const size_t kOffEditorText         = 0xe0;

static const uint32_t kProjectVersion      = 1;
static const uint32_t kPageVersion         = 1;
static const uint32_t kScriptBlockVersion  = 1;
static const uint32_t kBannerBytes         = 100;
static const uint32_t kFullPaletteBytes    = 0x400;  // 256 * sizeof(PALETTEENTRY)
static const int      kSignatureBias       = 0x21;   // stored byte = plain + 0x21

// External helpers referenced by the serializers (defined elsewhere in the exe).
extern void GraphBrdScript_RunGlobalSetupBlock(void *engine);
extern void GraphBrdScriptEngine_ParsePageScript(void *engine);
extern void GraphBrdView_CreateOrResizeBackBufferFromDib(void *view, const void *dib);
extern void ComponentList_Serialize(void *listDlg, CArchive *ar);   // vtable +0x08
extern void ScriptEngine_Serialize(void *engine, CArchive *ar);     // vtable +0x08
extern void ColeDocument_Serialize(CGraphBrdDoc *doc, CArchive *ar);
extern bool SignatureMatchesJulianTuwim(const CString *s);
extern void ReportEndOfMemoryAndQuit();
// Palette / DIB helpers factored out of the inlined MFC/GDI calls.
extern void   ReadAndAttachPagePalette(void *view, CArchive *ar, uint32_t byteCount);
extern void   WritePagePalette(void *view, CArchive *ar);      // 256 PALETTEENTRY
extern void  *ReadPackedDib(CArchive *ar, uint32_t byteCount); // GlobalAlloc+Read
extern uint32_t PackedDibByteCount(const void *dib);           // header+palette+pixels

// ===========================================================================
// 004230a0  GraphBrdScriptEditor_SerializeText
//
// Shared by the per-page script editor (doc +0x9c) and the group / global
// script editor (doc +0xa0). Disk layout: u32 version(=1) + CString text.
// The editor buffer lives at editor+0xe0 and has no trailing NUL on disk.
// ===========================================================================
void GraphBrdScriptEditor_SerializeText(CScriptEditorDlg *editor, CArchive *ar)
{
    CString *text = &Field<CString>(editor, kOffEditorText);

    if (CArchiveOps::IsStoring(ar)) {
        // UpdateData(TRUE) + SyncTextAndEnsureTrailingSpace pull the current
        // rich-edit contents into `text` before writing.
        CArchiveOps::WriteU32(ar, kScriptBlockVersion);
        CStringOps::Insert(ar, text);
        return;
    }

    CStringOps::Empty(text);
    (void)CArchiveOps::ReadU32(ar);           // version, ignored on load
    CStringOps::Extract(ar, text);
    // Original then pushes the text into the rich-edit control (EM_* + SetWindowText).
}

// ===========================================================================
// 00406020  GraphBrdDoc_SerializeProjectState  ->  START.PRJ
//
//   u32 version(=1)
//   CString startupPage
//   CString currentPageOrGroupState
//   u32 audioPresetIndex               (audioManager+0x20)
//   u32 pageCount;  CString pageNames[pageCount]
//   u32 groupCount; CString groupNames[groupCount]
//   CString signature                  ("Julian Tuwim", each byte + 0x21 on save)
//   <group/global script editor (doc +0xa0) Serialize>:
//       u32 scriptVersion(=1)
//       CString globalScript
//
// The trailing block is the global project script, NOT audio-manager state;
// the audio manager only contributes audioPresetIndex. On load the host runs
// the recovered global script through GraphBrdScript_RunGlobalSetupBlock.
// ===========================================================================
BOOL GraphBrdDoc_SerializeProjectState(CGraphBrdDoc *doc, CArchive *ar)
{
    void      *audioMgr   = Field<void *>(doc, kOffAudioManager);
    CString **&pageTable  = Field<CString **>(doc, kOffPageNamesPtr);  // array of CString*
    CString **&groupTable = Field<CString **>(doc, kOffGroupNamesPtr); // array of CString*
    int       &pageCount  = Field<int>(doc, kOffPageCount);
    int       &groupCount = Field<int>(doc, kOffGroupCount);
    CString   *sig        = &Field<CString>(doc, kOffSignature);

    if (CArchiveOps::IsStoring(ar)) {
        CArchiveOps::WriteU32(ar, kProjectVersion);
        CStringOps::Insert(ar, &Field<CString>(doc, kOffStartupPage));
        CStringOps::Insert(ar, &Field<CString>(doc, kOffCurrentState));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(audioMgr, kOffAudioPresetSlot));

        CArchiveOps::WriteU32(ar, static_cast<uint32_t>(pageCount));
        for (int i = 0; i < pageCount; ++i) {
            CStringOps::Insert(ar, pageTable[i]);
        }
        CArchiveOps::WriteU32(ar, static_cast<uint32_t>(groupCount));
        for (int i = 0; i < groupCount; ++i) {
            CStringOps::Insert(ar, groupTable[i]);
        }

        // Obfuscate the signature (+0x21) only for the duration of the write,
        // then restore the in-memory plaintext.
        for (int i = 0, n = CStringOps::GetLength(sig); i < n; ++i) {
            CStringOps::SetAt(sig, i, static_cast<char>(CStringOps::GetAt(sig, i) + kSignatureBias));
        }
        CStringOps::Insert(ar, sig);
        for (int i = 0, n = CStringOps::GetLength(sig); i < n; ++i) {
            CStringOps::SetAt(sig, i, static_cast<char>(CStringOps::GetAt(sig, i) - kSignatureBias));
        }
    } else {
        // Free any previously loaded name tables before reading fresh ones.
        int hadPages = pageCount;
        for (int i = 0; i < pageCount; ++i) {
            if (pageTable[i]) { CStringOps::Destruct(pageTable[i]); operator_delete(pageTable[i]); }
        }
        if (pageCount) operator_delete(pageTable);

        for (int i = 0; i < groupCount; ++i) {
            if (groupTable[i]) { CStringOps::Destruct(groupTable[i]); operator_delete(groupTable[i]); }
        }
        if (groupCount) operator_delete(groupTable);

        (void)CArchiveOps::ReadU32(ar);       // version, ignored
        CStringOps::Extract(ar, &Field<CString>(doc, kOffStartupPage));
        CStringOps::Extract(ar, &Field<CString>(doc, kOffCurrentState));
        Field<uint32_t>(audioMgr, kOffAudioPresetSlot) = CArchiveOps::ReadU32(ar);

        pageCount = static_cast<int>(CArchiveOps::ReadU32(ar));
        pageTable = pageCount ? static_cast<CString **>(operator_new(pageCount * 4)) : nullptr;
        for (int i = 0; i < pageCount; ++i) {
            pageTable[i] = static_cast<CString *>(operator_new(4));
            CStringOps::Construct(pageTable[i]);
            CStringOps::Extract(ar, pageTable[i]);
        }

        groupCount = static_cast<int>(CArchiveOps::ReadU32(ar));
        groupTable = groupCount ? static_cast<CString **>(operator_new(groupCount * 4)) : nullptr;
        for (int i = 0; i < groupCount; ++i) {
            groupTable[i] = static_cast<CString *>(operator_new(4));
            CStringOps::Construct(groupTable[i]);
            CStringOps::Extract(ar, groupTable[i]);
        }

        // De-obfuscate the signature (-0x21) and validate. The original guards
        // this on the pre-read page count being nonzero.
        if (hadPages) {
            CStringOps::Extract(ar, sig);
            for (int i = 0, n = CStringOps::GetLength(sig); i < n; ++i) {
                CStringOps::SetAt(sig, i, static_cast<char>(CStringOps::GetAt(sig, i) - kSignatureBias));
            }
            if (!SignatureMatchesJulianTuwim(sig)) {
                ReportEndOfMemoryAndQuit();   // original PostMessage(WM_CLOSE) path
            }
        }
    }

    // Trailing block: group/global script editor writes u32 version + CString.
    GraphBrdScriptEditor_SerializeText(Field<CScriptEditorDlg *>(doc, kOffGroupScriptEditor), ar);

    if (!CArchiveOps::IsStoring(ar)) {
        GraphBrdScript_RunGlobalSetupBlock(Field<void *>(doc, kOffScriptEngine));
    }
    return 1;
}

// ===========================================================================
// 004066f0  GraphBrdDoc_SerializePageData  ->  .BDF page files
//
//   char banner[100]                   ("YDP Board data file. Ver:%d 1996...")
//   u32 version(=1)
//   i32 pageLeft, pageTop, pageRight, pageBottom
//   i32 minLayer, maxLayer
//   u32 backgroundFlag
//   u8  backgroundColorIndex
//   u32 paletteByteCount; BYTE palette[paletteByteCount]   (0x400 when present)
//   u32 dibByteCount;     BYTE dib[dibByteCount]           (packed DIB)
//   <page component list (doc +0xa4) Serialize>
//   <page script editor  (doc +0x9c) Serialize>
//   <script engine       (doc +0xb4) Serialize>
//   COleDocument::Serialize
// ===========================================================================
void GraphBrdDoc_SerializePageData(CGraphBrdDoc *doc, CArchive *ar)
{
    void *view = /* GetActiveView via vtable +0x68/+0x6c */ doc;
    char  banner[kBannerBytes];

    if (!CArchiveOps::IsStoring(ar)) {
        // Release a previously loaded packed DIB, if any.
        HGLOBAL &dibSlot = Field<HGLOBAL>(doc, kOffPageDibPacked);
        if (dibSlot) {
            GlobalUnlock(GlobalHandle(dibSlot));
            GlobalFree(GlobalHandle(dibSlot));
            dibSlot = nullptr;
        }

        CArchiveOps::Read(ar, banner, kBannerBytes);
        (void)CArchiveOps::ReadU32(ar);                          // version, ignored

        Field<int>(doc, kOffPageLeft)   = static_cast<int>(CArchiveOps::ReadU32(ar));
        Field<int>(doc, kOffPageTop)    = static_cast<int>(CArchiveOps::ReadU32(ar));
        Field<int>(doc, kOffPageRight)  = static_cast<int>(CArchiveOps::ReadU32(ar));
        Field<int>(doc, kOffPageBottom) = static_cast<int>(CArchiveOps::ReadU32(ar));
        Field<int>(doc, kOffPageMinLayer) = static_cast<int>(CArchiveOps::ReadU32(ar));
        Field<int>(doc, kOffPageMaxLayer) = static_cast<int>(CArchiveOps::ReadU32(ar));
        Field<uint32_t>(doc, kOffPageBackgroundFlag) = CArchiveOps::ReadU32(ar);
        Field<uint8_t>(doc, kOffPageBgColorIndex)    = CArchiveOps::ReadU8(ar);

        uint32_t paletteBytes = CArchiveOps::ReadU32(ar);
        if (paletteBytes) {
            ReadAndAttachPagePalette(view, ar, paletteBytes);
        }

        uint32_t dibBytes = CArchiveOps::ReadU32(ar);
        if (dibBytes == 0) {
            if (Field<int>(doc, kOffPageRight) && Field<int>(doc, kOffPageBottom)) {
                GraphBrdView_CreateOrResizeBackBufferFromDib(view, nullptr);
            }
        } else {
            void *dib = ReadPackedDib(ar, dibBytes);
            Field<void *>(doc, kOffPageDibPacked) = dib;
            if (!dib) ReportEndOfMemoryAndQuit();
            GraphBrdView_CreateOrResizeBackBufferFromDib(view, dib);
        }
        // (original then recomputes view scroll/centering rects from the page rect)
    } else {
        // Banner is sprintf'd as "YDP Board data file. Ver:%d 1996..." plus a
        // CTime::Format("%A, %B %d, %Y") modification date, padded to 100 bytes.
        CArchiveOps::Write(ar, banner, kBannerBytes);
        CArchiveOps::WriteU32(ar, kPageVersion);

        CArchiveOps::WriteU32(ar, Field<uint32_t>(doc, kOffPageLeft));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(doc, kOffPageTop));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(doc, kOffPageRight));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(doc, kOffPageBottom));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(doc, kOffPageMinLayer));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(doc, kOffPageMaxLayer));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(doc, kOffPageBackgroundFlag));
        CArchiveOps::WriteU8(ar, Field<uint8_t>(doc, kOffPageBgColorIndex));

        void *paletteObj = /* view GDI palette at view+0x64 */ nullptr;
        if (paletteObj == nullptr) {
            CArchiveOps::WriteU32(ar, 0);
        } else {
            CArchiveOps::WriteU32(ar, kFullPaletteBytes);
            WritePagePalette(view, ar);
        }

        void *dib = Field<void *>(doc, kOffPageDibPacked);
        if (dib == nullptr) {
            CArchiveOps::WriteU32(ar, 0);
        } else {
            uint32_t dibBytes = PackedDibByteCount(dib);
            CArchiveOps::WriteU32(ar, dibBytes);
            CArchiveOps::Write(ar, dib, dibBytes);
        }
    }

    // Tail: page component list, page script text, script engine, then base doc.
    ComponentList_Serialize(Field<void *>(doc, kOffPageComponentList), ar);
    GraphBrdScriptEditor_SerializeText(Field<CScriptEditorDlg *>(doc, kOffPageScriptEditor), ar);
    ScriptEngine_Serialize(Field<void *>(doc, kOffScriptEngine), ar);
    if (Field<int>(doc, kOffScriptParsedFlag) == 0) {
        GraphBrdScriptEngine_ParsePageScript(Field<void *>(doc, kOffScriptEngine));
    }
    ColeDocument_Serialize(doc, ar);
}

// ===========================================================================
// 004064c0  GraphBrdDoc_SerializeCursorAndGroupState  ->  .GRP
//
//   u32 cursorCount
//   repeat cursorCount:
//       BYTE cursorRecord[0x2c]         (raw in-memory record; +0x28 pixel ptr
//                                        is stale on disk and rebuilt on load)
//       u32  pixelByteCount
//       BYTE pixels[pixelByteCount]
//   <group component-list dialog (doc +0xa8) Serialize>
// ===========================================================================
BOOL GraphBrdDoc_SerializeCursorAndGroupState(CGraphBrdDoc *doc, CArchive *ar)
{
    void **cursorTable = &Field<void *>(doc, kOffCursorTable);
    int   &count       = Field<int>(doc, kOffCursorCount);

    if (CArchiveOps::IsStoring(ar)) {
        CArchiveOps::WriteU32(ar, static_cast<uint32_t>(count));
        for (int i = 0; i < count; ++i) {
            void *record = cursorTable[i];
            CArchiveOps::Write(ar, record, kCursorRecordSize);
            uint32_t pixelBytes =
                static_cast<uint32_t>(GlobalSize(GlobalHandle(Field<LPVOID>(record, kOffCursorPixels))));
            CArchiveOps::WriteU32(ar, pixelBytes);
            CArchiveOps::Write(ar, Field<LPVOID>(record, kOffCursorPixels), pixelBytes);
        }
    } else {
        // Release the previously loaded cursor table.
        for (int i = 0; i < count; ++i) {
            void *record = cursorTable[i];
            LPVOID pixels = Field<LPVOID>(record, kOffCursorPixels);
            GlobalUnlock(GlobalHandle(pixels));
            GlobalFree(GlobalHandle(pixels));
            operator_delete(record);
        }

        count = static_cast<int>(CArchiveOps::ReadU32(ar));
        for (int i = 0; i < count; ++i) {
            void *record = operator_new(kCursorRecordSize);
            cursorTable[i] = record;
            CArchiveOps::Read(ar, record, kCursorRecordSize);

            uint32_t pixelBytes = CArchiveOps::ReadU32(ar);
            LPVOID pixels = GlobalLock(GlobalAlloc(2, pixelBytes));
            Field<LPVOID>(record, kOffCursorPixels) = pixels;
            if (pixels == nullptr) {
                ReportEndOfMemoryAndQuit();
                return 0;
            }
            CArchiveOps::Read(ar, pixels, pixelBytes);
        }
    }

    ComponentList_Serialize(Field<void *>(doc, kOffGroupComponentList), ar);
    return 1;
}

// ===========================================================================
// 0042b920  GraphBrdCntrItem_Serialize  ->  component wrapper records
//
// Serialized once per component inside every page/group component list:
//
//   u32 wrapperVersion(=1)
//   u32 reflectedFunctionCount
//   CString displayName                 (e.g. "Transparent_Video_Holder")
//   u32 reflectedPropertyCount
//   BYTE clsid[16]                      (duplicate of the ComponentList CLSID)
//   FunctionMemberRecord[reflectedFunctionCount]
//   PropertyMemberRecord[reflectedPropertyCount]
//   <component-private state: interface vtable slot +0x44>
//
// The member records are a reflection cache of the component's IDispatch
// surface; on load the dispatch-member cache is cleared and rebuilt from them.
// ===========================================================================

// In-memory member records. Pointer-sized fields keep the original x86 layout
// only on 32-bit targets; the serializers below never rely on sizeof(record).
struct FunctionMemberRecord {            // operator_new(0x1c)
    uint32_t dispatchIdOrOffset;         // +0x00
    uint32_t rawNameByteCount;           // +0x04
    char    *rawName;                    // +0x08, rawNameByteCount+1 bytes incl. NUL
    CString *typeOrReturnName;           // +0x0c
    CString *displayName;                // +0x10
    CString *descriptionOrHelpName;      // +0x14
    uint16_t flagsOrInvokeKind;          // +0x18
};

struct PropertyMemberRecord {            // operator_new(0x14)
    uint32_t rawNameByteCount;           // +0x00
    char    *rawName;                    // +0x04, rawNameByteCount+1 bytes incl. NUL
    uint32_t variantTypeOrDispatchMetadata; // +0x08
    CString *displayName;                // +0x0c
    uint16_t flagsOrInvokeKind;          // +0x10
};

// CGraphBrdCntrItem field offsets (COleClientItem-derived).
static const size_t kOffItemFunctionTable  = 0x20;   // FunctionMemberRecord*[50] inline
static const size_t kOffItemFunctionCount  = 0xe8;
static const size_t kOffItemDisplayName    = 0xf0;   // CString
static const size_t kOffItemClsid          = 0xf4;   // 16 raw bytes
static const size_t kOffItemPropertyCount  = 0x104;
static const size_t kOffItemPropertyTable  = 0x108;  // PropertyMemberRecord** (GlobalAlloc)
static const size_t kOffItemComponentItf   = 0x10c;  // component interface pointer
static const uint32_t kWrapperVersion      = 1;

extern void GraphBrdCntrItem_ClearDispatchMemberCache(void *item);
extern void ComponentInterface_Serialize(void *itf, CArchive *ar);  // vtable +0x44
extern uint16_t CArchiveOps_ReadU16(CArchive *ar);   // ar >> WORD
extern void     CArchiveOps_WriteU16(CArchive *ar, uint16_t v);

static CString *NewArchiveCString(CArchive *ar)
{
    CString *s = static_cast<CString *>(operator_new(4));
    CStringOps::Construct(s);
    CStringOps::Extract(ar, s);
    return s;
}

void GraphBrdCntrItem_Serialize(void *item, CArchive *ar)
{
    int &functionCount = Field<int>(item, kOffItemFunctionCount);
    int &propertyCount = Field<int>(item, kOffItemPropertyCount);
    FunctionMemberRecord **functionTable =
        &Field<FunctionMemberRecord *>(item, kOffItemFunctionTable);
    PropertyMemberRecord **&propertyTable =
        Field<PropertyMemberRecord **>(item, kOffItemPropertyTable);

    if (!CArchiveOps::IsStoring(ar)) {
        GraphBrdCntrItem_ClearDispatchMemberCache(item);

        (void)CArchiveOps::ReadU32(ar);              // wrapperVersion, ignored
        functionCount = static_cast<int>(CArchiveOps::ReadU32(ar));
        CStringOps::Extract(ar, &Field<CString>(item, kOffItemDisplayName));
        propertyCount = static_cast<int>(CArchiveOps::ReadU32(ar));
        CArchiveOps::Read(ar, &Field<uint8_t>(item, kOffItemClsid), 0x10);

        for (int i = 0; i < functionCount; ++i) {
            FunctionMemberRecord *rec =
                static_cast<FunctionMemberRecord *>(operator_new(0x1c));
            functionTable[i] = rec;
            rec->dispatchIdOrOffset = CArchiveOps::ReadU32(ar);
            rec->rawNameByteCount   = CArchiveOps::ReadU32(ar);
            rec->rawName = static_cast<char *>(operator_new(rec->rawNameByteCount + 1));
            CArchiveOps::Read(ar, rec->rawName, rec->rawNameByteCount + 1);
            rec->typeOrReturnName      = NewArchiveCString(ar);
            rec->displayName           = NewArchiveCString(ar);
            rec->descriptionOrHelpName = NewArchiveCString(ar);
            rec->flagsOrInvokeKind     = CArchiveOps_ReadU16(ar);
        }

        propertyTable = static_cast<PropertyMemberRecord **>(
            GlobalLock(GlobalAlloc(0x42, propertyCount * 4 + 4)));
        for (int i = 0; i < propertyCount; ++i) {
            PropertyMemberRecord *rec =
                static_cast<PropertyMemberRecord *>(operator_new(0x14));
            propertyTable[i] = rec;
            rec->rawNameByteCount = CArchiveOps::ReadU32(ar);
            rec->rawName = static_cast<char *>(operator_new(rec->rawNameByteCount + 1));
            CArchiveOps::Read(ar, rec->rawName, rec->rawNameByteCount + 1);
            rec->variantTypeOrDispatchMetadata = CArchiveOps::ReadU32(ar);
            rec->displayName       = NewArchiveCString(ar);
            rec->flagsOrInvokeKind = CArchiveOps_ReadU16(ar);
        }
    } else {
        CArchiveOps::WriteU32(ar, kWrapperVersion);
        CArchiveOps::WriteU32(ar, static_cast<uint32_t>(functionCount));
        CStringOps::Insert(ar, &Field<CString>(item, kOffItemDisplayName));
        CArchiveOps::WriteU32(ar, static_cast<uint32_t>(propertyCount));
        CArchiveOps::Write(ar, &Field<uint8_t>(item, kOffItemClsid), 0x10);

        for (int i = 0; i < functionCount; ++i) {
            FunctionMemberRecord *rec = functionTable[i];
            CArchiveOps::WriteU32(ar, rec->dispatchIdOrOffset);
            CArchiveOps::WriteU32(ar, rec->rawNameByteCount);
            CArchiveOps::Write(ar, rec->rawName, rec->rawNameByteCount + 1);
            CStringOps::Insert(ar, rec->typeOrReturnName);
            CStringOps::Insert(ar, rec->displayName);
            CStringOps::Insert(ar, rec->descriptionOrHelpName);
            CArchiveOps_WriteU16(ar, rec->flagsOrInvokeKind);
        }

        for (int i = 0; i < propertyCount; ++i) {
            PropertyMemberRecord *rec = propertyTable[i];
            CArchiveOps::WriteU32(ar, rec->rawNameByteCount);
            CArchiveOps::Write(ar, rec->rawName, rec->rawNameByteCount + 1);
            CArchiveOps::WriteU32(ar, rec->variantTypeOrDispatchMetadata);
            CStringOps::Insert(ar, rec->displayName);
            CArchiveOps_WriteU16(ar, rec->flagsOrInvokeKind);
        }
    }

    // Component-private state: the component DLL's serialize method sits at
    // interface vtable slot +0x44 and reads/writes directly after the records.
    void *componentItf = Field<void *>(item, kOffItemComponentItf);
    if (componentItf != nullptr) {
        ComponentInterface_Serialize(componentItf, ar);
    }
}

// ===========================================================================
// 0041aad0  GraphBrdScriptEngine_Serialize  ->  parsed-script index state
//
// Appears in .BDF right after the page script text (document field +0xb4).
// It caches the parse results for the page script so the runtime can jump to
// switch blocks and built-in calls without re-scanning the text. All offsets
// are character offsets into the page script text.
//
//   u32 schemaVersion(=4)
//   u32 switchBlockCount
//   u32 engine+0x38, engine+0x3c, engine+0x30, engine+0x34   (parser state)
//   repeat switchBlockCount:                (SwitchCaseRecord, 0x14 in memory)
//       u32 blockStart                      (written twice; both copies are
//       u32 blockStart                       rec+0x00, second wins on load)
//       u32 caseCount
//       u32 blockEnd
//       u32 defaultBodyOffset
//       repeat caseCount: u32 caseValue, u32 caseBodyOffset
//   u32 builtinCallCount
//   u32 builtinTokenOffsets[builtinCallCount]   (raw block, engine+0x11b4)
//   u32 builtinCallKinds[builtinCallCount]      (raw block, engine+0x1344)
//   if schema >= 2: u32 engine+0x44, +0x48, +0x4c, +0x50
//   if schema >= 3: u32 engine+0x54
//   if schema >= 4: u32 engine+0x58
//
// Tables are built by GraphBrdScript_IndexSwitchCaseBlocks (00426480) and
// GraphBrdScript_IndexBuiltInCalls (00427dd0); the call records feed
// GraphBrdScript_InvokeIndexedCall at runtime. Both tables cap at 100 entries.
// ===========================================================================
struct SwitchCaseEntry {                 // operator_new(8)
    uint32_t caseValue;
    uint32_t caseBodyOffset;
};

struct SwitchCaseRecord {                // operator_new(0x14)
    uint32_t blockStart;                 // +0x00
    uint32_t caseCount;                  // +0x04
    SwitchCaseEntry **cases;             // +0x08 (GlobalAlloc'd pointer array)
    uint32_t blockEnd;                   // +0x0c
    uint32_t defaultBodyOffset;          // +0x10
};

// CGraphBrdScriptEngine field offsets.
static const size_t kOffEngineSwitchTable   = 0x1020;  // SwitchCaseRecord*[100] inline
static const size_t kOffEngineSwitchCount   = 0x11b0;
static const size_t kOffEngineTokenOffsets  = 0x11b4;  // u32[100]
static const size_t kOffEngineCallKinds     = 0x1344;  // u32[100]
static const size_t kOffEngineCallCount     = 0x14d4;
static const size_t kOffEngineRuntimeFlag   = 0x14e0;  // cleared on load
// Serialized scalar parser state; exact roles not yet recovered.
static const size_t kOffEngineState30 = 0x30, kOffEngineState34 = 0x34;
static const size_t kOffEngineState38 = 0x38, kOffEngineState3c = 0x3c;
static const size_t kOffEngineState44 = 0x44, kOffEngineState48 = 0x48;
static const size_t kOffEngineState4c = 0x4c, kOffEngineState50 = 0x50;
static const size_t kOffEngineState54 = 0x54, kOffEngineState58 = 0x58;
static const uint32_t kScriptEngineSchema = 4;

void GraphBrdScriptEngine_Serialize(void *engine, CArchive *ar)
{
    SwitchCaseRecord **switchTable = &Field<SwitchCaseRecord *>(engine, kOffEngineSwitchTable);
    int      &switchCount  = Field<int>(engine, kOffEngineSwitchCount);
    int      &callCount    = Field<int>(engine, kOffEngineCallCount);
    uint32_t *tokenOffsets = &Field<uint32_t>(engine, kOffEngineTokenOffsets);
    uint32_t *callKinds    = &Field<uint32_t>(engine, kOffEngineCallKinds);

    if (!CArchiveOps::IsStoring(ar)) {
        // Free the previous switch index before reading the new one.
        for (int i = 0; i < switchCount; ++i) {
            SwitchCaseRecord *rec = switchTable[i];
            for (uint32_t j = 0; j < rec->caseCount; ++j) {
                operator_delete(rec->cases[j]);
            }
            GlobalUnlock(GlobalHandle(rec->cases));
            GlobalFree(GlobalHandle(rec->cases));
            operator_delete(rec);
        }

        uint32_t schema = CArchiveOps::ReadU32(ar);
        switchCount = static_cast<int>(CArchiveOps::ReadU32(ar));
        Field<uint32_t>(engine, kOffEngineState38) = CArchiveOps::ReadU32(ar);
        Field<uint32_t>(engine, kOffEngineState3c) = CArchiveOps::ReadU32(ar);
        Field<uint32_t>(engine, kOffEngineState30) = CArchiveOps::ReadU32(ar);
        Field<uint32_t>(engine, kOffEngineState34) = CArchiveOps::ReadU32(ar);

        for (int i = 0; i < switchCount; ++i) {
            SwitchCaseRecord *rec =
                static_cast<SwitchCaseRecord *>(operator_new(0x14));
            switchTable[i] = rec;
            rec->blockStart = CArchiveOps::ReadU32(ar);   // legacy copy
            rec->blockStart = CArchiveOps::ReadU32(ar);   // current copy wins
            rec->caseCount  = CArchiveOps::ReadU32(ar);
            rec->blockEnd   = CArchiveOps::ReadU32(ar);
            rec->defaultBodyOffset = CArchiveOps::ReadU32(ar);
            rec->cases = static_cast<SwitchCaseEntry **>(
                GlobalLock(GlobalAlloc(0x42, rec->caseCount * 4)));
            for (uint32_t j = 0; j < rec->caseCount; ++j) {
                SwitchCaseEntry *entry =
                    static_cast<SwitchCaseEntry *>(operator_new(8));
                rec->cases[j] = entry;
                entry->caseValue      = CArchiveOps::ReadU32(ar);
                entry->caseBodyOffset = CArchiveOps::ReadU32(ar);
            }
        }

        callCount = static_cast<int>(CArchiveOps::ReadU32(ar));
        if (callCount != 0) {
            CArchiveOps::Read(ar, tokenOffsets, callCount * 4);
            CArchiveOps::Read(ar, callKinds, callCount * 4);
        }

        if (schema > 1) {
            Field<uint32_t>(engine, kOffEngineState44) = CArchiveOps::ReadU32(ar);
            Field<uint32_t>(engine, kOffEngineState48) = CArchiveOps::ReadU32(ar);
            Field<uint32_t>(engine, kOffEngineState4c) = CArchiveOps::ReadU32(ar);
            Field<uint32_t>(engine, kOffEngineState50) = CArchiveOps::ReadU32(ar);
        }
        if (schema > 2) {
            Field<uint32_t>(engine, kOffEngineState54) = CArchiveOps::ReadU32(ar);
        }
        if (schema > 3) {
            Field<uint32_t>(engine, kOffEngineState58) = CArchiveOps::ReadU32(ar);
        }
        Field<uint32_t>(engine, kOffEngineRuntimeFlag) = 0;
    } else {
        CArchiveOps::WriteU32(ar, kScriptEngineSchema);
        CArchiveOps::WriteU32(ar, static_cast<uint32_t>(switchCount));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(engine, kOffEngineState38));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(engine, kOffEngineState3c));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(engine, kOffEngineState30));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(engine, kOffEngineState34));

        for (int i = 0; i < switchCount; ++i) {
            SwitchCaseRecord *rec = switchTable[i];
            CArchiveOps::WriteU32(ar, rec->blockStart);
            CArchiveOps::WriteU32(ar, rec->blockStart);
            CArchiveOps::WriteU32(ar, rec->caseCount);
            CArchiveOps::WriteU32(ar, rec->blockEnd);
            CArchiveOps::WriteU32(ar, rec->defaultBodyOffset);
            for (uint32_t j = 0; j < rec->caseCount; ++j) {
                CArchiveOps::WriteU32(ar, rec->cases[j]->caseValue);
                CArchiveOps::WriteU32(ar, rec->cases[j]->caseBodyOffset);
            }
        }

        CArchiveOps::WriteU32(ar, static_cast<uint32_t>(callCount));
        if (callCount != 0) {
            CArchiveOps::Write(ar, tokenOffsets, callCount * 4);
            CArchiveOps::Write(ar, callKinds, callCount * 4);
        }
        CArchiveOps::WriteU32(ar, Field<uint32_t>(engine, kOffEngineState44));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(engine, kOffEngineState48));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(engine, kOffEngineState4c));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(engine, kOffEngineState50));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(engine, kOffEngineState54));
        CArchiveOps::WriteU32(ar, Field<uint32_t>(engine, kOffEngineState58));
    }
}
