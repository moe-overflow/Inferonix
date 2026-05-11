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
            _total_time += (_delta_time * _time_scale);
        }

        auto static get_delta_time() -> float { return _delta_time; }
        auto static get_time() -> float { return _total_time; }

        /* raw delta time */
        auto static get_unscaled_delta_time() -> float { return _delta_time; }

        auto static set_time_scale(float const scale) -> void { _time_scale = scale; }
        auto static get_time_scale() -> float { return _time_scale; }

    private:
        inline static float _delta_time { .0f };
        inline static float _total_time { .0f };
        inline static float _time_scale { 1.0f };
        inline static time_point _last_frame_time;
    };
}