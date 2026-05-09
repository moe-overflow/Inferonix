#pragma once

#include <imgui.h>
#include <vector>

#include "layer.hpp"

namespace inferonix::ui
{
    class dev_console final : public layer
    {
    public:
        dev_console() : _current_y(-1000.0f) {}

        auto on_render() -> void override
        {
            if (ImGui::IsKeyPressed(ImGuiKey_M, false))
            {
                _is_open = !_is_open;
                if (_is_open)
                {
                    _reclaim_focus = true;
                }
            }


            auto const delta_time = ImGui::GetIO().DeltaTime;

            auto const console_height = ImGui::GetIO().DisplaySize.y * 0.5;
            auto const window_width = ImGui::GetIO().DisplaySize.x;

            auto const target_y = _is_open ? 0.0f : -static_cast<float>(console_height);

            constexpr float slide_speed = 15.0f;
            _current_y += (target_y - _current_y) * slide_speed * delta_time;

            // todo: return if it's off screen

            using namespace ImGui;
            SetNextWindowPos({ 0, _current_y}, ImGuiCond_Always);
            SetNextWindowSize({ window_width, static_cast<float>(console_height) }, ImGuiCond_Always);
            SetNextWindowBgAlpha(0.9f);

            constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

            Begin("DevConsole", nullptr, flags);
            {
                float const footer_height = GetStyle().ItemSpacing.y + GetFrameHeightWithSpacing();
                BeginChild("Logs", {0, -footer_height},false, ImGuiWindowFlags_HorizontalScrollbar);
                for (const auto& log : _history) TextUnformatted(log.c_str());
                EndChild();


                PushItemWidth(-1);

                if (_reclaim_focus)
                {
                    SetKeyboardFocusHere();
                    _reclaim_focus = false;
                }

                if (InputText("##ConsoleInput", _input_buffer, IM_ARRAYSIZE(_input_buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    // todo: execute command
                    SetKeyboardFocusHere(-1);
                    _reclaim_focus = true;
                }
                PopItemWidth();
            }
            End();
        }

    private:
        bool _is_open {false};
        float _current_y {.0f};

        char _input_buffer[256] = "";
        std::vector<std::string> _history;

        bool _reclaim_focus { false };

    };
}

