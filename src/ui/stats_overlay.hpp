#pragma once

#include "layer.hpp"
#include "util/time.hpp"

#include <imgui.h>
#include <cstdio>

namespace inferonix::ui
{
    class stats_overlay final : public layer
    {
    public:
        stats_overlay() = default;

        auto on_render() -> void override
        {
            if (!_visible)
                return;

            const auto dt = utils::time::get_delta_time();
            _accumulator += dt;
            _frame_count++;

            if (_accumulator >= 1.0f)
            {
                _display_fps = static_cast<int>(static_cast<float>(_frame_count) / _accumulator);
                _display_frame_time_ms = (dt * 1000.0f);
                _accumulator = 0.0f;
                _frame_count = 0;
            }

            auto* draw_list = GetForegroundDrawList();
            const auto display_size = GetIO().DisplaySize;
            const auto font_size = GetFontSize();
            const auto padding = 12.0f;
            const auto spacing = 24.0f;

            char buf_fps[32];
            char buf_ms[32];
            std::snprintf(buf_fps, sizeof(buf_fps), "FPS: %d", _display_fps);
            std::snprintf(buf_ms, sizeof(buf_ms), "%.2f ms", _display_frame_time_ms);

            const auto text_fps = CalcTextSize(buf_fps);
            const auto text_ms = CalcTextSize(buf_ms);
            const auto total_width = text_fps.x + spacing + text_ms.x;
            const auto x_start = (display_size.x - total_width) * 0.5f;
            const auto y = display_size.y - padding - font_size;

            constexpr auto text_color = IM_COL32(220, 220, 220, 220);
            constexpr auto dim_color = IM_COL32(160, 160, 160, 180);

            draw_list->AddText(ImVec2(x_start, y), text_color, buf_fps);
            draw_list->AddText(ImVec2(x_start + text_fps.x + spacing, y), dim_color, buf_ms);
        }

        static auto toggle() -> void { _visible = !_visible; }
        [[nodiscard]] static auto is_visible() -> bool { return _visible; }

    private:
        static inline bool _visible = true;

        float _accumulator = 0.0f;
        int _frame_count = 0;
        int _display_fps = 0;
        float _display_frame_time_ms = 0.0f;
    };
}
