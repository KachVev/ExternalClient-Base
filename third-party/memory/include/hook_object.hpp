#pragma once
#include <MinHook.h>
#include <iostream>

template<typename T>
class InlineHook {
public:
    bool Add(void* targetFn, T detourFn) {
        if (MH_CreateHook(targetFn, reinterpret_cast<void*>(detourFn), reinterpret_cast<void**>(&original)) != MH_OK)
            return false;
        if (MH_EnableHook(targetFn) != MH_OK)
            return false;
        target = targetFn;
        return true;
    }

    void Remove() {
        if (target) {
            MH_DisableHook(target);
            MH_RemoveHook(target);
            target = nullptr;
            original = nullptr;
        }
    }

    T GetOriginal() const { return original; }

private:
    void* target = nullptr;
    T original = nullptr;
};
