#include "layer.hpp"

#include "imgui.h"

using namespace inferonix::ui;
using namespace ImGui;

void dockspace::on_render()
{
    static auto dockspace_flags = ImGuiDockNodeFlags_None; //ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    auto const viewport = GetMainViewport();
    SetNextWindowPos(viewport->WorkPos);
    SetNextWindowSize(viewport->WorkSize);
    SetNextWindowViewport(viewport->ID);
    PushStyleVar(ImGuiStyleVar_WindowRounding, .0f);
    PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
                    | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
                    | ImGuiWindowFlags_NoNavFocus;


    //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(.0f, .0f));
    bool active = true;
    Begin("Dockspace", &active, window_flags);

    PopStyleVar(2);

    //ImGui::PopStyleVar(1);

    if (GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        auto const dockspace_id = GetID("MainDockspace");
        DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    End();
}

