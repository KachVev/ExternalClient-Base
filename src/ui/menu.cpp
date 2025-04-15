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

    void RenderTopBar(const ImVec2& winPos, const ImVec2& winSize) {
        constexpr float headerHeight = 60.0f;
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        drawList->AddLine(
            ImVec2(winPos.x, winPos.y + headerHeight),
            ImVec2(winPos.x + winSize.x, winPos.y + headerHeight),
            ImColor(24, 25, 36),
            2.0f
        );

        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(35, 35, 45, 255).Value);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(50, 50, 60, 255).Value);

        constexpr float buttonSize = 28.0f;
        const float buttonX = winPos.x + winSize.x - buttonSize - 16.0f;
        const float buttonY = winPos.y + (headerHeight - buttonSize) * 0.5f;

        ImGui::SetCursorScreenPos(ImVec2(buttonX, buttonY));
        if (ImGui::InvisibleButton("##SettingsBtn", ImVec2(buttonSize, buttonSize))) {
            std::cout << "Test" << std::endl;
        }

        const ImVec2 iconPos = ImGui::GetItemRectMin();
        const ImVec2 iconSize = ImGui::CalcTextSize(ICON_FA_GEAR);
        const auto iconCenter = ImVec2(
            iconPos.x + (buttonSize - iconSize.x) * 0.5f,
            iconPos.y + (buttonSize - iconSize.y) * 0.5f + 3.0f
        );

        drawList->AddText(iconCenter, ImColor(255, 255, 255, 255), ICON_FA_GEAR);

        ImGui::PopStyleColor(3);
        ImGui::PopFont();
    }


    void Render() {
        SetupStyle();

        ImGui::SetNextWindowSize(ImVec2(900, 500), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(300, 200), ImGuiCond_Once);
        bool open = true;

        ImGui::Begin("##MainWindow",
            &open,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse);

        ImVec2 winPos = ImGui::GetWindowPos();
        ImVec2 winSize = ImGui::GetWindowSize();

        RenderTopBar(winPos, winSize);


        ImGui::End();
    }

}