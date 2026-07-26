#pragma once

#include <filesystem>
#include "window/window.hpp"

namespace inferonix
{
    struct inferonix_engine_config final
    {
        window::window_settings window_settings =
        {
            .width = 1920,
            .height = 1080,
            .title = "Inferonix Engine",
            .full_screen = false,
            .v_sync = true
        };
        std::filesystem::path scene_file = "assets/scenes/level01.json";
        bool headless { false };

    };
}

