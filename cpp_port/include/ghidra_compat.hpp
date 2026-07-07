#pragma once

#include <cstddef>
#include <cstdint>

using byte = std::uint8_t;
using undefined = std::uint8_t;
using undefined1 = std::uint8_t;
using undefined2 = std::uint16_t;
using undefined3 = std::uint32_t;
using undefined4 = std::uint32_t;
using undefined8 = std::uint64_t;
using uint = unsigned int;
using ulong = unsigned long;

struct AFX_MODULE_STATE;
struct CArchive;
struct CCmdTarget;
struct CDialog;
struct CFile;
struct COleDocument;
struct CObject;
struct CPropertyPage;
struct CPropertySheet;
struct CString;
struct CWnd;

using BOOL = int;
using CHAR = char;
using DWORD = std::uint32_t;
using DWORD_PTR = std::uintptr_t;
using HANDLE = void *;
using HBRUSH__ = void;
using HGLOBAL = void *;
using HICON__ = void;
using HMENU__ = void;
using HWND__ = void;
using HWAVEOUT = void *;
using LPCVOID = const void *;
using LPBYTE = byte *;
using LPDWORD = DWORD *;
using LPHWAVEOUT = HWAVEOUT *;
using LPOVERLAPPED = void *;
using LPVOID = void *;
using LPWAVEHDR = void *;
using MMRESULT = unsigned int;
using SIZE_T = std::size_t;
using UINT = unsigned int;
using WORD = std::uint16_t;

struct WAVEFORMATEX {
  WORD wFormatTag;
  WORD nChannels;
  DWORD nSamplesPerSec;
  DWORD nAvgBytesPerSec;
  WORD nBlockAlign;
  WORD wBitsPerSample;
  WORD cbSize;
};

extern void *ExceptionList;
