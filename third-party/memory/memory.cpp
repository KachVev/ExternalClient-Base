#include "include/memory.hpp"
#include <tlhelp32.h>
#include <iostream>

Memory::Memory(const std::wstring& processName) : processName(processName) {
    if (const DWORD pid = getPid(processName)) {
        process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        if (process) {
            std::wcout << L"[Memory] Opened " << processName << L" (PID: " << pid << L")\n";
            processId = pid;
        } else
            std::wcerr << L"[Memory] Failed to open process. Error: " << GetLastError() << L"\n";
    } else {
        std::wcerr << L"[Memory] Process not found: " << processName << L"\n";
    }
}

Memory::~Memory() {
    if (process)
        CloseHandle(process);
}

uintptr_t Memory::getModule(const std::wstring& moduleName) const {
    if (!process) {
        throw std::runtime_error("[Memory] Invalid process handle.");
    }

    const DWORD pid = getPid(processName);
    HANDLE modSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (modSnapshot == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("[Memory] Failed to create module snapshot. Error: " + std::to_string(GetLastError()));
    }

    MODULEENTRY32W modEntry = { sizeof(MODULEENTRY32W) };
    uintptr_t moduleBase = 0;

    if (Module32FirstW(modSnapshot, &modEntry)) {
        do {
            if (_wcsicmp(modEntry.szModule, moduleName.c_str()) == 0) {
                moduleBase = reinterpret_cast<uintptr_t>(modEntry.modBaseAddr);
                break;
            }
        } while (Module32NextW(modSnapshot, &modEntry));
    } else {
        CloseHandle(modSnapshot);
        throw std::runtime_error("[Memory] Failed to retrieve the first module from the snapshot. Error: " + std::to_string(GetLastError()));
    }

    CloseHandle(modSnapshot);
    return moduleBase;
}

size_t Memory::getModuleSize(const std::wstring& moduleName) const {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    MODULEENTRY32W entry = { sizeof(entry) };
    if (Module32FirstW(snapshot, &entry)) {
        do {
            if (moduleName == entry.szModule) {
                CloseHandle(snapshot);
                return entry.modBaseSize;
            }
        } while (Module32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return 0;
}

DWORD Memory::getPid(const std::wstring& name) {
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("[Memory] Failed to create process snapshot. Error: " + std::to_string(GetLastError()));
    }

    PROCESSENTRY32W procEntry = { sizeof(PROCESSENTRY32W) };

    if (Process32FirstW(snapshot, &procEntry)) {
        do {
            if (_wcsicmp(procEntry.szExeFile, name.c_str()) == 0) {
                pid = procEntry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &procEntry));
    } else {
        CloseHandle(snapshot);
        throw std::runtime_error("[Memory] Failed to retrieve the first process from the snapshot. Error: " + std::to_string(GetLastError()));
    }

    CloseHandle(snapshot);
    return pid;
}
