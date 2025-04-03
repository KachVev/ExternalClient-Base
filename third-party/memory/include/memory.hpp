#pragma once
#include <Windows.h>
#include <iostream>
#include <string>

class Memory {
public:
    HANDLE process = nullptr;
    DWORD processId = 0;
    std::wstring processName;


    explicit Memory(const std::wstring& processName);
    ~Memory();

    template<typename T>
    T read(const uintptr_t address) const {
        T buffer{};
        if (!ReadProcessMemory(process, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), nullptr)) {
            throw std::runtime_error("Failed to read memory at address: " + std::to_string(address));
        }
        return buffer;
    }

    void readRaw(const uintptr_t address, void* buffer, const size_t size) const {
        ReadProcessMemory(process, reinterpret_cast<LPCVOID>(address), buffer, size, nullptr);
    }

    uintptr_t getModule(const std::wstring& moduleName) const;
    uintptr_t getModuleSize(const std::wstring& moduleName) const;

private:
    static DWORD getPid(const std::wstring& name);
};