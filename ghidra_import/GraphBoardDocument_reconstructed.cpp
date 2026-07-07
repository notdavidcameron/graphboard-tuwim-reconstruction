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
