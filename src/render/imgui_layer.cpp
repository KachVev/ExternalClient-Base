#include "imgui_layer.hpp"
#include <d3d11.h>
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
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
    pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    gDevice->CreateRenderTargetView(backBuffer, nullptr, &gRTV);
    backBuffer->Release();
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
