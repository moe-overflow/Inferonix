#include "main_panel.hpp"

#include "imgui.h"

using namespace Inferonix::UI;

constexpr static auto MAIN_PANEL_NAME = "MAIN_PANEL";

main_panel::main_panel() :
    panel(MAIN_PANEL_NAME)
{}

void main_panel::display()
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    auto viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0F);

    flags |= ImGuiWindowFlags_NoTitleBar |
             ImGuiWindowFlags_NoCollapse |
             ImGuiTableColumnFlags_NoResize |
             ImGuiWindowFlags_NoMove |
             ImGuiWindowFlags_NoBringToFrontOnFocus |
             ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("Main", nullptr, flags);
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0F, 0.0F), ImGuiDockNodeFlags_None);

    ImGui::End();
}




