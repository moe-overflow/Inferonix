#include "ui_manager.hpp"
#include <spdlog/spdlog.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

using namespace Inferonix::UI;

void ui_manager::init()
{
    spdlog::info("initializing ImGUI");

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;


    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable
            // | ImGuiConfigFlags_NavEnableKeyboard
    ;

    ImGui_ImplGlfw_InitForOpenGL(_window->get_glfw_context(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core"); // todo: get shading version from window class

    ImGui::StyleColorsDark();

}


void ui_manager::begin_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

}

void ui_manager::end_frame()
{
    ImGui::Render();
    int width, height;

    glfwGetFramebufferSize(_window->get_glfw_context(), &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void ui_manager::terminate()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ui_manager::render_panels()
{
    for(const auto& panel : _panels)
    {
        panel->display();
    }
}

void ui_manager::add_panel(std::unique_ptr<panel> panel)
{
    _panels.emplace_back(std::move(panel));
}

void ui_manager::display_ui()
{
    begin_frame();
    render_panels();
    end_frame();
}

