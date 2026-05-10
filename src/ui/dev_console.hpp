#pragma once

#include <imgui.h>
#include <vector>
#include <string_view>
#include <mutex>

#include "layer.hpp"
#include "util/command_registry.hpp"

namespace inferonix::ui
{
    class dev_console final : public layer
    {
    public:
        using execute_callback = std::function<void(const std::string&)>;
        explicit dev_console(execute_callback const& on_execute) : _on_execute(on_execute), _current_y(-1000.0f)
        {}

        auto on_render() -> void override
        {
            if (ImGui::IsKeyPressed(ImGuiKey_F1, false))
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

            if (!_is_open && _current_y <= -static_cast<float>(console_height) + 1.0f) return;

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
                bool const is_at_bottom = GetScrollY() >= GetScrollMaxY() - 5.0f;
                {
                    auto lock = std::lock_guard<std::mutex>{ _history_mutex };
                    for (const auto& log : _history)
                        TextUnformatted(log.c_str());
                }
                if (is_at_bottom || _reclaim_focus)
                    SetScrollHereY(1.0f);

                EndChild();

                PushItemWidth(-1);

                if (_reclaim_focus)
                {
                    SetKeyboardFocusHere();
                    _reclaim_focus = false;
                }

                if (InputText("##ConsoleInput", _input_buffer, IM_ARRAYSIZE(_input_buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {

                    /*
                    auto const raw = std::string { _input_buffer };
                    insert_log(">>> " + raw);

                    auto stream = std::istringstream{ raw };
                    std::string command_name;
                    stream >> command_name;

                    auto args = std::vector<std::string>{};
                    auto arg = std::string{};
                    while (stream >> arg)
                        args.push_back(arg);
                    */

                    auto const command = std::string{ _input_buffer };
                    if (!command.empty() && _on_execute)
                        _on_execute(command);
                    _input_buffer[0] = '\0';

                    SetKeyboardFocusHere(-1);
                    _reclaim_focus = true;
                }
                PopItemWidth();
            }
            End();
        }

        [[nodiscard]] auto is_open() const -> bool { return _is_open; }

        auto insert_log(const std::string_view log_message) -> void
        {
            auto lock = std::lock_guard<std::mutex>{ _history_mutex };
            _history.emplace_back(log_message.data());
            if (_history.size() > 500)
                _history.erase(_history.begin());
        } 


    private:
        execute_callback _on_execute;

        bool _is_open {false};
        float _current_y {.0f};

        char _input_buffer[256] = "";

        // log history
        std::vector<std::string> _history;

        bool _reclaim_focus { false };
        std::mutex _history_mutex;

    };
}

