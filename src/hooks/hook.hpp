#pragma once

#include <Windows.h>
#include <dxgi.h>
#include <hook_object.hpp>

namespace Hooks {
    void Init();
    void Cleanup();

    namespace Handles {
        HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

        inline InlineHook<decltype(&hkPresent)> Present{};
    }
}
