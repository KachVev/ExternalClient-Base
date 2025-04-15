#include "imgui_layer.hpp"
#include <d3d11.h>
#include <filesystem>
#include <iostream>
#include <Windows.h>
#include <__msvc_ostream.hpp>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "../fonts/IconsFontAwesome6.h"
#include "../ui/menu.hpp"

ID3D11Device* gDevice = nullptr;
ID3D11DeviceContext* gContext = nullptr;
ID3D11RenderTargetView* gRTV = nullptr;
HWND gHwnd = nullptr;
bool gInit = false;
bool gShowMenu = false;

static WNDPROC oWndProc = nullptr;


LRESULT CALLBACK hkWndProc(const HWND hwnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) {
    if (gShowMenu) {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
            return true;
    }

    return CallWindowProc(oWndProc, hwnd, msg, wParam, lParam);
}

void HookWndProc(const HWND hwnd) {
    oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hkWndProc)));
}

void CreateRTV(IDXGISwapChain* pSwapChain) {
    ID3D11Texture2D* backBuffer;
    pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    gDevice->CreateRenderTargetView(backBuffer, nullptr, &gRTV);
    backBuffer->Release();
}

std::string GetDllPath() {
    HMODULE hModule = nullptr;
    GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCSTR>(&GetDllPath),
        &hModule
    );
    char path[MAX_PATH];
    GetModuleFileNameA(hModule, path, MAX_PATH);
    return std::string(path);
}

void SetupFonts() {
    const ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();

    const std::filesystem::path dllPath = GetDllPath();
    const std::filesystem::path baseDir = dllPath.parent_path();
    const std::filesystem::path fontDir = baseDir / "fonts";

    const std::string iconFontPath = (fontDir / "awesome.ttf").string();
    const std::string poppinsFontPath = (fontDir / "poppins.ttf").string();

    std::cout << iconFontPath << std::endl;
    io.Fonts->AddFontFromFileTTF(poppinsFontPath.c_str(), 16.0f);

    static constexpr ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;

    io.Fonts->AddFontFromFileTTF(iconFontPath.c_str(), 18.0f, &icons_config, icons_ranges);

    io.Fonts->Build();
}

void RenderImGui(IDXGISwapChain* pSwapChain) {

    if (!gInit) {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&gDevice)))) {
            gDevice->GetImmediateContext(&gContext);
            DXGI_SWAP_CHAIN_DESC desc;
            pSwapChain->GetDesc(&desc);
            gHwnd = desc.OutputWindow;

            HookWndProc(gHwnd);

            ImGui::CreateContext();
            ImGui_ImplWin32_Init(gHwnd);
            ImGui_ImplDX11_Init(gDevice, gContext);
            ImGui::StyleColorsDark();
            CreateRTV(pSwapChain);
            SetupFonts();
            gInit = true;
        }
    }

    if (gInit) {
        if (GetAsyncKeyState(VK_INSERT) & 1)
            gShowMenu = !gShowMenu;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (gShowMenu)
            Menu::Render();

        ImGui::Render();
        gContext->OMSetRenderTargets(1, &gRTV, nullptr);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}
