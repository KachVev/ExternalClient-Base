#include "hook.hpp"
#include <iostream>
#include <MinHook.h>
#include "memory.hpp"
#include "../render/imgui_layer.hpp"

using namespace Hooks;

HRESULT __stdcall Handles::hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    RenderImGui(pSwapChain);
    return Present.GetOriginal()(pSwapChain, SyncInterval, Flags);
}

void Hooks::Init() {
    std::cout << "[*] Initializing hooks...\n";

    if (MH_Initialize() != MH_OK) {
        std::cout << "[-] MH_Initialize failed\n";
        return;
    }

    void* presentAddr = Memory::patternScan(
        "GameOverlayRenderer64.dll",
        "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 41 8B E8"
    );

    if (!Handles::Present.Add(presentAddr, &Handles::hkPresent)) {
        std::cout << "[-] Failed to hook Present.\n";
        return;
    }

    std::cout << "[+] Hooked Present successfully.\n";
}

void Hooks::Cleanup() {
    Handles::Present.Remove();
    MH_Uninitialize();
    std::cout << "[*] Hooks cleaned up.\n";
}