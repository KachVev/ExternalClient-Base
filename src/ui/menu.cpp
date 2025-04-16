#include <imgui.h>
#include "menu.hpp"
#include <iostream>
#include "../fonts/IconsFontAwesome6.h"

namespace Menu {
    void SetupStyle() {
        static bool styleInit = false;
        if (styleInit) return;

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 7.0f;
        style.WindowBorderSize = 0.0f;
        style.FrameBorderSize = 0.0f;
        style.WindowPadding = ImVec2(16, 16);
        style.Colors[ImGuiCol_WindowBg] = ImColor(19, 20, 31);
        style.Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);

        styleInit = true;
    }


    void RenderTopBar(const ImVec2& pos, const ImVec2& size) {
        constexpr float h = 60.0f, btn = 28.0f;
        auto* dl = ImGui::GetWindowDrawList();

        dl->AddLine({pos.x, pos.y + h}, {pos.x + size.x, pos.y + h}, ImColor(24, 25, 36), 2.0f);

        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::PushStyleColor(ImGuiCol_Button,        {0, 0, 0, 0});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(35, 35, 45).Value);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImColor(50, 50, 60).Value);

        ImGui::SetCursorScreenPos({pos.x + size.x - btn - 16.0f, pos.y + (h - btn) * 0.5f});
        if (ImGui::InvisibleButton("##SettingsBtn", {btn, btn}))
            std::cout << "Test\n";

        ImVec2 iconPos = ImGui::GetItemRectMin();
        ImVec2 iconSize = ImGui::CalcTextSize(FontAwesome::ICON_FA_GEAR);
        ImVec2 iconCenter = {iconPos.x + (btn - iconSize.x) * 0.5f, iconPos.y + (btn - iconSize.y) * 0.5f + 3.0f};

        dl->AddText(iconCenter, ImColor(255, 255, 255), FontAwesome::ICON_FA_GEAR);

        ImGui::PopStyleColor(3);
        ImGui::PopFont();
    }

    void RenderLeftBar(const ImVec2& pos, const ImVec2& size) {
        constexpr float h = 60.0f, offset = 180.0f, thickness = 3.0f;
        auto* dl = ImGui::GetWindowDrawList();

        dl->AddLine(
            {pos.x + offset, pos.y + h + thickness / 2.0f},
            {pos.x + offset, pos.y + size.y},
            ImColor(24, 25, 36),
            thickness
        );
    }

    void Render() {
        SetupStyle();
        ImGui::SetNextWindowSize({900, 500}, ImGuiCond_Once);
        ImGui::SetNextWindowPos({300, 200}, ImGuiCond_Once);

        bool open = true;
        ImGui::Begin("##MainWindow", &open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        ImVec2 pos = ImGui::GetWindowPos();
        ImVec2 size = ImGui::GetWindowSize();

        RenderTopBar(pos, size);
        RenderLeftBar(pos, size);

        ImGui::End();
    }

}
