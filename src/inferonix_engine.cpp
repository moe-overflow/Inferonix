#include "inferonix_engine.hpp"

#include "inferonix_pch.hpp"

#include "scene/components.hpp"
#include "renderer/renderer.hpp"
#include "scene/scene_serializer.hpp"

#include "ui/layer.hpp"
#include "ui/dev_console.hpp"
#include "ui/console_sink.hpp"
#include "ui/toolbar.hpp"
#include "ui/scene_hierarchy.hpp"

#include "util/time.hpp"

#include <fmt/format.h>
#include <argparse/argparse.hpp>

using namespace inferonix;
using namespace inferonix::window;
using namespace inferonix::renderer;
using namespace inferonix::scene;

namespace
{
    auto log_scene_registry(registry& registry) -> void
    {
        const auto view = registry.view<tag_component>();
        LOG(LOG_TYPE::INFO, "Scene has following entities: ");
        for (const auto entity : view)
        {
            const auto& tag = view.get<tag_component>(entity).tag;
            auto id = std::string{"no ID"};
            if (registry.any_of<id_component>(entity))
                id = registry.get<id_component>(entity).id;
            LOG(LOG_TYPE::INFO, "We have entity: {}, with ID: {}", tag, id);

            if (registry.any_of<transform_component>(entity))
            {
                const auto& transform_ = registry.get<transform_component>(entity);
                LOG(LOG_TYPE::INFO, "Entity has transform: {}, {}, {}",
                    transform_.position.x, transform_.position.y, transform_.position.z
                );
            }
        }
        spdlog::info("");
    }
}

inferonix_engine::inferonix_engine(std::unique_ptr<inferonix_engine_config> const& config)
    : _window(std::make_shared<window::window>(config->window_settings)),
      _renderer(std::make_shared<renderer::renderer>(_window)),
      _scene(std::make_shared<scene::scene>()),
      _scene_serializer(std::make_unique<scene_serializer>(*_scene))
{
    this->init(config->scene_file.string());
}

inferonix_engine::~inferonix_engine()
{
    if (_initialized)
    {
        _script_launcher.terminate();
    }
}

void inferonix_engine::init(const std::string_view scene_path)
{
    add_window_layers();
    register_commands();
    attach_dev_console_log_sink();
    _renderer->setup();
    _physics_engine.init();
    _script_launcher.launch();
    _scene_serializer->deserialize(scene_path.data());
    auto& registry = _scene->get_registry();
    log_scene_registry(registry);
    _physics_engine.start_simulation(registry);
    _initialized = true;
}

void inferonix_engine::run()
{
    while (!_window->should_close())
    {
        assert(_initialized);

        utils::time::update();
        window::window::poll_events();

        auto& console = _window->get_layer<ui::dev_console>();
        _scene->get_editor_camera()->set_block_input(console.is_open());

        utils::time::set_time_scale(console.is_open() || _state == EngineState::PAUSE ? 0.0f : 1.0f);

        _script_launcher.start(_scene->get_registry());

        if (console.is_open())
            _state = EngineState::PAUSE;

        auto const dt = utils::time::get_delta_time();
        if(_state == EngineState::PLAY)
        {
            _script_launcher.update(_scene->get_registry(), dt);
            _physics_engine.update(_scene->get_registry(), dt);
        }

        _scene->get_editor_camera()->update(dt);

        renderer::renderer::clear();
        _renderer->render(*_scene);
        _window->display();
        _window->swap_buffers();
    }
}

void inferonix_engine::attach_dev_console_log_sink() const
{
    // create a spdlog sink pointing to the dev console
    configure_console_sink([this](const std::string& message) -> void {
        _window->get_layer<ui::dev_console>().insert_log(message);
    });
}


void inferonix_engine::add_window_layers()
{
    _window->add_layer<ui::dockspace>();
    _window->add_layer<ui::dev_console>(
        [this](const std::string_view command) -> void { _command_registry.execute(command); }
    );
    _window->add_layer<ui::toolbar>(
        [this]() -> void { _state = EngineState::PLAY;  },
        [this]() -> void { _state = EngineState::PAUSE; }
    );
    _window->add_layer<ui::scene_hierarchy>( _scene->get_registry() );
    _window->add_layer<ui::scene_layer>(_renderer->get_frame_buffer(), _scene, _window->get_layer<ui::scene_hierarchy>());
}

void inferonix_engine::register_commands()
{

    auto parse_bool_switch = [](std::string_view value) -> std::optional<bool>
    {
        if (value == "1" || value == "true" || value == "on")
            return true;

        if (value == "0" || value == "false" || value == "off")
            return false;

        return std::nullopt;
    };

    _command_registry.register_command(
            "quit", "Exits the engine", [this](const auto& args)-> void { _window->close(); }
    );

    _command_registry.register_command(
        "wireframe", "wireframe_mode", [this, parse_bool_switch](const auto& args)-> void
        {
            if (args.empty())
            {
                _renderer->toggle_wireframe_mode();
                spdlog::info("Wireframe toggled");
                return;
            }
            auto value = parse_bool_switch(args[0]);
            if (!value)
            {
                spdlog::error("Expected on/off, true/false or 1/0");
                return;
            }
            _renderer->set_wireframe_mode(*value);
            spdlog::info("Wireframe mode set to {}", *value ? "ON" : "OFF");

        }
    );
}


auto inferonix::parse_command_line(int argc, char** argv) -> std::unique_ptr<inferonix_engine_config>
{
    auto config = inferonix_engine_config{};

    auto parser = argparse::ArgumentParser("Inferonix Engine");
    parser.add_argument("--headless").default_value(false).implicit_value(true);
    parser.add_argument("--scene");

    parser.add_argument("--width").scan<'i', int>();
    parser.add_argument("--height").scan<'i', int>();

    parser.parse_args(argc, argv);

    if (parser.is_used("--headless"))
        config.headless = parser.get<bool>("--headless");

    if (parser.is_used("--scene"))
        config.scene_file = parser.get<std::string>("--scene");

    if (parser.is_used("--width"))
        config.window_settings.width = parser.get<int>("--width");

    if (parser.is_used("--height"))
        config.window_settings.height = parser.get<int>("--height");

    return std::make_unique<inferonix_engine_config>(config);
}


