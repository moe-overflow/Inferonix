#pragma once

#include <memory>

#include "renderer/renderer.hpp"
#include "window/window.hpp"
#include "script/scripting_engine_launcher.hpp"
#include "physics/physics.hpp"

namespace inferonix
{

    class inferonix_engine final
    {
    public:
        inferonix_engine();

        inferonix_engine(inferonix_engine const&) = delete;
        inferonix_engine(inferonix_engine&&) = delete;

        inferonix_engine& operator=(inferonix_engine const&) = delete;
        inferonix_engine& operator=(inferonix_engine&&) = delete;

        ~inferonix_engine();

        void Run();

    private:
        std::shared_ptr<window::window> _window;
        std::shared_ptr<renderer::renderer> _renderer;
        std::unique_ptr<scene::scene> _scene;

        script::scripting_engine_launcher _script_launcher;
        bool _initialized{ false };

        physics::physics_engine _physics_engine;
    };

}

