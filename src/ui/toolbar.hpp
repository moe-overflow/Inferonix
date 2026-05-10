#pragma once

#include "layer.hpp"

#include <functional>
#include <imgui.h>

namespace inferonix::ui
{
    class toolbar final : public layer
    {
    public:
        using action_callback = std::function<void()>;

        toolbar(action_callback on_play, action_callback on_pause)
                    : _on_play(std::move(on_play)), _on_pause(std::move(on_pause)) {}

        ~toolbar() override = default;

        auto on_render() -> void override
        {
            using namespace ImGui;
            Begin("Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar);
            if (Button("Play"))  if (_on_play) _on_play();
            SameLine();
            if (Button("Pause")) if (_on_pause) _on_pause();
            End();
        }

    private:
        action_callback _on_play;
        action_callback _on_pause;
    };
}
