#include "../include/memory.hpp"
#include <tlhelp32.h>
#include <iostream>
#include <Psapi.h>

Memory::Memory() {
    processId_ = GetCurrentProcessId();
    std::wcout << L"[Memory] Current PID: " << processId_ << L'\n';
}

uintptr_t Memory::getModule(const std::wstring& moduleName) {
    HMODULE modules[1024];
    DWORD needed;

    if (!EnumProcessModules(GetCurrentProcess(), modules, sizeof(modules), &needed)) {
        std::wcerr << L"[Memory] Failed to enumerate modules.\n";
        return 0;
    }

    const auto count = needed / sizeof(HMODULE);
    for (size_t i = 0; i < count; ++i) {
        wchar_t modName[MAX_PATH];
        if (GetModuleBaseNameW(GetCurrentProcess(), modules[i], modName, std::size(modName))) {
            if (_wcsicmp(modName, moduleName.c_str()) == 0) {
                return reinterpret_cast<uintptr_t>(modules[i]);
            }
        }
    }

    std::wcerr << L"[Memory] Failed to find module: " << moduleName << L'\n';
    return 0;
}

void* Memory::findPattern(const uintptr_t moduleBase, const char* pattern) {
#define INRANGE(x,a,b)    ((x) >= (a) && (x) <= (b))
#define GETBITS(x)        (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA) : (INRANGE(x, '0', '9') ? (x - '0') : 0))
#define GETBYTE(x)        (GETBITS(x[0]) << 4 | GETBITS(x[1]))

    if (!moduleBase)
        return nullptr;

    auto start = reinterpret_cast<BYTE*>(moduleBase);
    const auto dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(start);
    const auto ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(start + dosHeader->e_lfanew);

    const size_t sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;

    const BYTE* end = start + sizeOfImage - 0x1000;
    start += 0x1000;

    const char* currentPattern = pattern;
    BYTE* firstMatch = nullptr;

    for (; start < end; ++start) {
        if (*currentPattern == '\0')
            break;

        if (const bool skipByte = (*currentPattern == '\?'); skipByte || *start == GETBYTE(currentPattern)) {
            if (!firstMatch)
                firstMatch = start;

            currentPattern += skipByte ? 2 : 3;

            if (currentPattern[-1] == '\0') {
                return firstMatch;
            }
        } else if (firstMatch) {
            start = firstMatch;
            firstMatch = nullptr;
            currentPattern = pattern;
        }
    }

    return nullptr;

#undef INRANGE
#undef GETBITS
#undef GETBYTE
}
