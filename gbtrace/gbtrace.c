// gbtrace.dll - a breakpoint tracer for the real Tuwim.exe / GraphBoard DLLs.
//
// Purpose: recover behaviors the C++ port only approximates (sprite glide
// speed, group/.grp loading, the video still-frame draw condition) by logging
// the real calls and their arguments as the game runs.
//
// Mechanism: a vectored exception handler plus INT3 (0xCC) software
// breakpoints. No trampolines or length-disassembly -- on a breakpoint we log
// the arguments off the stack, restore the original byte, single-step over the
// original instruction, then re-arm the breakpoint. This is robust across the
// __thiscall / __cdecl calling conventions the MFC component DLLs use.
//
// Targets are read from gbtrace.ini next to the injected DLL, so functions can
// be added without recompiling. Ghidra addresses are given at each module's
// preferred base; the tracer relocates them to the module's actual load base.
// Modules that load after injection (the component DLLs) are armed by a poll
// thread as they appear.

#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#define MAX_BPS 128
#define MAX_MODS 32

typedef struct {
    char     module[64];     // "" / "exe" => main image; else a DLL name
    uintptr_t ghidraAddr;    // address as shown in Ghidra (preferred base)
    uintptr_t preferredBase; // module preferred base for that address
    int       argc;          // stack args to log
    int       thiscall;      // 1 => also log ECX (the `this` pointer)
    char      name[96];      // label for the log
    // Runtime:
    uintptr_t actualAddr;    // resolved absolute address, 0 until armed
    BYTE      original;      // saved original byte under the 0xCC
    int       armed;
} Breakpoint;

static Breakpoint g_bps[MAX_BPS];
static int g_bpCount = 0;
static FILE* g_log = NULL;
static CRITICAL_SECTION g_lock;
static DWORD g_tlsPending;   // TLS slot: Breakpoint* awaiting re-arm after step

static void logf(const char* fmt, ...) {
    if (!g_log) return;
    EnterCriticalSection(&g_lock);
    va_list ap;
    va_start(ap, fmt);
    vfprintf(g_log, fmt, ap);
    va_end(ap);
    fflush(g_log);
    LeaveCriticalSection(&g_lock);
}

// Write `b` at `addr`, flipping page protection as needed.
static void pokeByte(uintptr_t addr, BYTE b) {
    DWORD old;
    if (VirtualProtect((LPVOID)addr, 1, PAGE_EXECUTE_READWRITE, &old)) {
        *(BYTE*)addr = b;
        FlushInstructionCache(GetCurrentProcess(), (LPVOID)addr, 1);
        VirtualProtect((LPVOID)addr, 1, old, &old);
    }
}

static uintptr_t moduleBase(const char* mod) {
    if (mod[0] == 0 || _stricmp(mod, "exe") == 0 || _stricmp(mod, "tuwim.exe") == 0) {
        return (uintptr_t)GetModuleHandleA(NULL);
    }
    return (uintptr_t)GetModuleHandleA(mod);
}

// Arm any breakpoint whose module is now loaded. Called at startup and from the
// poll thread as component DLLs come in.
static void armPending(void) {
    EnterCriticalSection(&g_lock);
    for (int i = 0; i < g_bpCount; ++i) {
        Breakpoint* bp = &g_bps[i];
        if (bp->armed) continue;
        uintptr_t base = moduleBase(bp->module);
        if (!base) continue;   // module not loaded yet
        bp->actualAddr = base + (bp->ghidraAddr - bp->preferredBase);
        bp->original = *(BYTE*)bp->actualAddr;
        pokeByte(bp->actualAddr, 0xCC);
        bp->armed = 1;
        logf("[arm] %s @ %p (base %p)\n", bp->name, (void*)bp->actualAddr, (void*)base);
    }
    LeaveCriticalSection(&g_lock);
}

static Breakpoint* findByAddr(uintptr_t addr) {
    for (int i = 0; i < g_bpCount; ++i) {
        if (g_bps[i].armed && g_bps[i].actualAddr == addr) return &g_bps[i];
    }
    return NULL;
}

// If `v` points to a readable, mostly-printable ASCII string, copy up to 63
// chars into out (quoted). MFC CStrings passed by value arrive as this char*.
static void previewString(uintptr_t v, char* out, int outsz) {
    out[0] = 0;
    if (v < 0x10000 || IsBadReadPtr((const void*)v, 1)) return;
    const char* p = (const char*)v;
    int n = 0, printable = 0;
    char buf[64];
    for (; n < 63; ++n) {
        if (IsBadReadPtr(p + n, 1)) break;
        char c = p[n];
        if (c == 0) break;
        buf[n] = (c >= 32 && c < 127) ? c : '.';
        if (c >= 32 && c < 127) printable++;
    }
    buf[n] = 0;
    if (n >= 2 && printable >= n * 3 / 4) {
        _snprintf(out, outsz, " \"%s\"", buf);
    }
}

static LONG CALLBACK vehHandler(EXCEPTION_POINTERS* ep) {
    const DWORD code = ep->ExceptionRecord->ExceptionCode;
    CONTEXT* ctx = ep->ContextRecord;

    if (code == EXCEPTION_BREAKPOINT) {
        Breakpoint* bp = findByAddr((uintptr_t)ep->ExceptionRecord->ExceptionAddress);
        if (!bp) return EXCEPTION_CONTINUE_SEARCH;

        // At entry ESP -> return address; stack args follow at ESP+4, +8, ...
        uintptr_t esp = ctx->Esp;
        uintptr_t ret = *(uintptr_t*)esp;
        char args[512];
        int n = 0;
        n += _snprintf(args + n, sizeof(args) - n, "ret=%p", (void*)ret);
        if (bp->thiscall) {
            n += _snprintf(args + n, sizeof(args) - n, " this=%p", (void*)ctx->Ecx);
        }
        for (int a = 0; a < bp->argc && n < (int)sizeof(args) - 96; ++a) {
            uintptr_t v = *(uintptr_t*)(esp + 4 + a * 4);
            char sp[80];
            previewString(v, sp, sizeof(sp));
            n += _snprintf(args + n, sizeof(args) - n, " a%d=%d(0x%x)%s", a, (int)v, (unsigned)v, sp);
        }
        logf("[hit] tid=%lu %s %s\n", GetCurrentThreadId(), bp->name, args);

        // Restore the original instruction, rewind EIP onto it, and single-step
        // so we can put the 0xCC back afterward.
        pokeByte(bp->actualAddr, bp->original);
        ctx->Eip = bp->actualAddr;
        ctx->EFlags |= 0x100;   // trap flag
        TlsSetValue(g_tlsPending, bp);
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    if (code == EXCEPTION_SINGLE_STEP) {
        Breakpoint* bp = (Breakpoint*)TlsGetValue(g_tlsPending);
        if (!bp) return EXCEPTION_CONTINUE_SEARCH;
        pokeByte(bp->actualAddr, 0xCC);   // re-arm
        TlsSetValue(g_tlsPending, NULL);
        ctx->EFlags &= ~0x100;
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

static void trim(char* s) {
    char* p = s + strlen(s);
    while (p > s && (p[-1] == '\r' || p[-1] == '\n' || p[-1] == ' ' || p[-1] == '\t')) *--p = 0;
    while (*s == ' ' || *s == '\t') memmove(s, s + 1, strlen(s));
}

// Config line: module addr preferredBase argc thiscall name...
//   module: "exe" for Tuwim.exe, or a DLL filename.
//   Lines starting with ; or # are comments.
static void loadConfig(const char* dir) {
    char path[MAX_PATH];
    _snprintf(path, sizeof(path), "%s\\gbtrace.ini", dir);
    FILE* f = fopen(path, "r");
    if (!f) { logf("[cfg] no gbtrace.ini at %s\n", path); return; }
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        trim(line);
        if (line[0] == 0 || line[0] == ';' || line[0] == '#') continue;
        if (g_bpCount >= MAX_BPS) break;
        Breakpoint* bp = &g_bps[g_bpCount];
        memset(bp, 0, sizeof(*bp));
        char name[96] = {0};
        int matched = sscanf(line, "%63s %I64x %I64x %d %d %95[^\n]",
                             bp->module, (unsigned __int64*)&bp->ghidraAddr,
                             (unsigned __int64*)&bp->preferredBase, &bp->argc,
                             &bp->thiscall, name);
        if (matched < 5) { logf("[cfg] skip: %s\n", line); continue; }
        _snprintf(bp->name, sizeof(bp->name), "%s", name[0] ? name : bp->module);
        g_bpCount++;
    }
    fclose(f);
    logf("[cfg] loaded %d breakpoints\n", g_bpCount);
}

static DWORD WINAPI pollThread(LPVOID unused) {
    (void)unused;
    for (;;) {
        armPending();
        // Stop polling once everything is armed.
        int allArmed = 1;
        for (int i = 0; i < g_bpCount; ++i) if (!g_bps[i].armed) allArmed = 0;
        if (allArmed) break;
        Sleep(200);
    }
    logf("[poll] all breakpoints armed\n");
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID reserved) {
    (void)reserved;
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinst);
        InitializeCriticalSection(&g_lock);
        g_tlsPending = TlsAlloc();

        // Log + config live next to this DLL.
        char dir[MAX_PATH];
        GetModuleFileNameA(hinst, dir, sizeof(dir));
        char* slash = strrchr(dir, '\\');
        if (slash) *slash = 0;
        char logpath[MAX_PATH];
        _snprintf(logpath, sizeof(logpath), "%s\\gbtrace.log", dir);
        g_log = fopen(logpath, "w");
        logf("[init] gbtrace attached, pid=%lu\n", GetCurrentProcessId());

        AddVectoredExceptionHandler(1, vehHandler);
        loadConfig(dir);
        armPending();
        CreateThread(NULL, 0, pollThread, NULL, 0, NULL);
    }
    return TRUE;
}
