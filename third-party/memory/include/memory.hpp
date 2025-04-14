#pragma once

#include <Windows.h>
#include <string>

class Memory {
public:
    Memory();
    ~Memory() = default;

    [[nodiscard]] static uintptr_t getModule(const std::wstring& moduleName);
    [[nodiscard]] static void* findPattern(uintptr_t moduleBase, const char* pattern);

    static void* patternScan(const std::string& moduleName, const char* pattern) {
        return findPattern(getModule(std::wstring(moduleName.begin(), moduleName.end())), pattern);
    }

private:
    DWORD processId_{};
};
