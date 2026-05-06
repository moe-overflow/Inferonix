#pragma once

#include <memory>

#include "renderer/renderer.hpp"
#include "window/window.hpp"
#include "script/scripting_engine_launcher.hpp"
#include "physics/physics.hpp"

namespace Inferonix
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
        std::shared_ptr<Window::window> _window;
        std::shared_ptr<Renderer::renderer> _renderer;
        std::unique_ptr<Scene::scene> _scene;

        Script::scripting_engine_launcher _script_launcher;
        bool _initialized{ false };

        Physics::PhysicsEngine _physics_engine;
    };

}

