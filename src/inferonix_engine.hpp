#pragma once

#include <memory>

#include "renderer/renderer.hpp"
#include "window/window.hpp"
#include "script/scripting_engine_launcher.hpp"
#include "physics/physics.hpp"
#include "util/command_registry.hpp"

namespace inferonix
{
    namespace scene {
        class scene_serializer;
    }

    enum class EngineState
    {
        PAUSE,
        PLAY,
    };

    class inferonix_engine final
    {
    public:
        inferonix_engine();

        inferonix_engine(inferonix_engine const&) = delete;
        inferonix_engine(inferonix_engine&&) = delete;

        inferonix_engine& operator=(inferonix_engine const&) = delete;
        inferonix_engine& operator=(inferonix_engine&&) = delete;

        ~inferonix_engine();

        void init(std::string_view scene_path);
        void run();

        void configure_console_sink() const;


    private:
        std::shared_ptr<window::window> _window;
        std::shared_ptr<renderer::renderer> _renderer;
        std::unique_ptr<scene::scene> _scene;

        utils::command_registry _command_registry;

        script::scripting_engine_launcher _script_launcher;
        bool _initialized{ false };

        physics::physics_engine _physics_engine;

        std::unique_ptr<scene::scene_serializer> _scene_serializer;

        EngineState _state{ EngineState::PLAY };
        bool _was_console_open{ false };

        void register_commands();
        void add_window_layers();

    };

}

