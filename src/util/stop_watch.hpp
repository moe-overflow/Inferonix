#pragma once

#include <chrono>

namespace inferonix::utils
{
    struct stop_watch final
    {
        using time_point = std::chrono::steady_clock::time_point;

        auto static update() -> void
        {
            const auto current_time = std::chrono::steady_clock::now();
            auto const duration = std::chrono::duration<float>{ current_time - _last_frame_time };
            _last_frame_time = current_time;
            _delta_time = duration.count();

        }

        auto static get_delta_time() -> float { return _delta_time; }

    private:
        inline static float _delta_time;
        inline static time_point _last_frame_time;
    };
}