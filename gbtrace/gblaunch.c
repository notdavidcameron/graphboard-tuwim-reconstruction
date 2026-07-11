// gblaunch.exe - start Tuwim.exe (suspended), inject gbtrace.dll, resume.
//
// Usage: gblaunch <target.exe> <gbtrace.dll> [workdir]
// If omitted, defaults target to the sibling Tuwim.exe and the DLL to
// gbtrace.dll next to this launcher. The child's working directory defaults to
// the target's own folder (the game finds DATA / Tuwim.INI there).
//
// 32-bit: kernel32's LoadLibraryA sits at the same address in this process and
// the 32-bit child, so the remote-thread start address is valid as-is.

#include <windows.h>
#include <stdio.h>

static int fail(const char* what) {
    fprintf(stderr, "gblaunch: %s failed (err %lu)\n", what, GetLastError());
    return 1;
}

int main(int argc, char** argv) {
    char self[MAX_PATH];
    GetModuleFileNameA(NULL, self, sizeof(self));
    char selfDir[MAX_PATH];
    _snprintf(selfDir, sizeof(selfDir), "%s", self);
    char* slash = strrchr(selfDir, '\\');
    if (slash) *slash = 0;

    char target[MAX_PATH], dll[MAX_PATH], workdir[MAX_PATH];
    if (argc > 1) _snprintf(target, sizeof(target), "%s", argv[1]);
    else          _snprintf(target, sizeof(target), "%s\\Tuwim.exe", selfDir);
    if (argc > 2) _snprintf(dll, sizeof(dll), "%s", argv[2]);
    else          _snprintf(dll, sizeof(dll), "%s\\gbtrace.dll", selfDir);

    if (argc > 3) {
        _snprintf(workdir, sizeof(workdir), "%s", argv[3]);
    } else {
        _snprintf(workdir, sizeof(workdir), "%s", target);
        char* s = strrchr(workdir, '\\');
        if (s) *s = 0;
    }

    printf("gblaunch: target=%s\n         dll=%s\n         cwd=%s\n", target, dll, workdir);

    STARTUPINFOA si; ZeroMemory(&si, sizeof(si)); si.cb = sizeof(si);
    PROCESS_INFORMATION pi; ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcessA(target, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED,
                        NULL, workdir, &si, &pi)) {
        return fail("CreateProcess");
    }

    // Write the DLL path into the child and LoadLibraryA it from a remote thread.
    SIZE_T len = strlen(dll) + 1;
    void* remote = VirtualAllocEx(pi.hProcess, NULL, len, MEM_COMMIT, PAGE_READWRITE);
    if (!remote) { TerminateProcess(pi.hProcess, 1); return fail("VirtualAllocEx"); }
    if (!WriteProcessMemory(pi.hProcess, remote, dll, len, NULL)) {
        TerminateProcess(pi.hProcess, 1); return fail("WriteProcessMemory");
    }

    FARPROC loadLib = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    HANDLE th = CreateRemoteThread(pi.hProcess, NULL, 0,
                                   (LPTHREAD_START_ROUTINE)loadLib, remote, 0, NULL);
    if (!th) { TerminateProcess(pi.hProcess, 1); return fail("CreateRemoteThread"); }
    WaitForSingleObject(th, INFINITE);
    DWORD dllBase = 0;
    GetExitCodeThread(th, &dllBase);
    printf("gblaunch: injected (remote HMODULE=0x%lx)\n", dllBase);
    if (dllBase == 0) {
        fprintf(stderr, "gblaunch: LoadLibraryA returned NULL in the child; DLL not loaded\n");
        TerminateProcess(pi.hProcess, 1);
        return 1;
    }
    CloseHandle(th);
    VirtualFreeEx(pi.hProcess, remote, 0, MEM_RELEASE);

    ResumeThread(pi.hThread);
    printf("gblaunch: child resumed (pid %lu)\n", pi.dwProcessId);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
}
