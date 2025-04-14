#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include <imgui.h>
#include "menu.hpp"

#include <string>

namespace Menu {
    static ImVec2 pos = { 200, 200 };
    static ImVec2 dragOffset = { 0, 0 };
    static bool dragging = false;
    static int activeTab = 0;
    static bool hoveringInteractive = false;

    constexpr float MENU_WIDTH = 600.0f;
    constexpr float MENU_HEIGHT = 420.0f;
    constexpr float SIDEBAR_WIDTH = 60.0f;
    constexpr float ICON_SIZE = 26.0f;
    constexpr int ICON_COUNT = 6;
    constexpr float BORDER_RADIUS = 10.0f;

    const char* iconLabels[ICON_COUNT] = { "A", "B", "C", "D", "E", "F" };

    void HandleDragging() {
        const ImGuiIO& io = ImGui::GetIO();
        const ImVec2 mousePos = io.MousePos;
        ImVec2 menuEnd = pos + ImVec2(MENU_WIDTH, MENU_HEIGHT);

        bool hovered = mousePos.x >= pos.x && mousePos.x <= menuEnd.x &&
                       mousePos.y >= pos.y && mousePos.y <= menuEnd.y;

        if (hovered && ImGui::IsMouseClicked(0) && !hoveringInteractive) {
            dragging = true;
            dragOffset = mousePos - pos;
        }

        if (!ImGui::IsMouseDown(0))
            dragging = false;

        if (dragging)
            pos = mousePos - dragOffset;
    }

    void RenderSidebar(ImDrawList* draw) {
        ImVec2 sidebarEnd = pos + ImVec2(SIDEBAR_WIDTH, MENU_HEIGHT);
        draw->AddRectFilled(pos, sidebarEnd, IM_COL32(50, 50, 50, 200), BORDER_RADIUS, ImDrawFlags_RoundCornersLeft);

        float totalIconSpace = ICON_COUNT * ICON_SIZE;
        float spacing = (MENU_HEIGHT - totalIconSpace) / (ICON_COUNT + 1);

        ImGuiIO& io = ImGui::GetIO();
        hoveringInteractive = false;

        for (int i = 0; i < ICON_COUNT; ++i) {
            ImVec2 iconPos = pos + ImVec2(0, spacing * (i + 1) + ICON_SIZE * i);
            ImVec2 iconCenter = iconPos + ImVec2(SIDEBAR_WIDTH / 2, ICON_SIZE / 2);
            ImVec2 iconEnd = iconPos + ImVec2(SIDEBAR_WIDTH, ICON_SIZE);
            ImVec2 textSize = ImGui::CalcTextSize(iconLabels[i]);

            if (i == activeTab) {
                draw->AddRectFilled(iconPos + ImVec2(10, 0), iconPos + ImVec2(SIDEBAR_WIDTH - 10, ICON_SIZE),
                                    IM_COL32(100, 100, 255, 180), 6.0f);
            }

            draw->AddText(iconCenter - textSize * 0.5f, IM_COL32(200, 200, 255, 255), iconLabels[i]);

            bool hovered = io.MousePos.x >= iconPos.x && io.MousePos.x <= iconEnd.x &&
                           io.MousePos.y >= iconPos.y && io.MousePos.y <= iconEnd.y;

            if (hovered) {
                hoveringInteractive = true;

                if (ImGui::IsMouseClicked(0)) {
                    activeTab = i;
                }
            }
        }
    }

    void RenderMainPanel(ImDrawList* draw) {
        ImVec2 mainStart = pos + ImVec2(SIDEBAR_WIDTH, 0);
        ImVec2 mainEnd = pos + ImVec2(MENU_WIDTH, MENU_HEIGHT);

        draw->AddRectFilled(mainStart, mainEnd, IM_COL32(25, 25, 25, 255), BORDER_RADIUS, ImDrawFlags_RoundCornersRight);

        draw->AddText(mainStart + ImVec2(20, 20), IM_COL32(255, 255, 255, 255),
                      ("Main Panel: Tab " + std::to_string(activeTab)).c_str());
    }

    void Render() {
        hoveringInteractive = false;

        HandleDragging();
        ImDrawList* draw = ImGui::GetBackgroundDrawList();

        RenderSidebar(draw);
        RenderMainPanel(draw);
    }
}
