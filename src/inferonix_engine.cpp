#include "inferonix_engine.hpp"

#include "scene/components.hpp"
#include "renderer/renderer.hpp"
#include "scene/scene_serializer.hpp"

#include "ui/layer.hpp"
#include "ui/dev_console.hpp"
#include "ui/console_sink.hpp"
#include "ui/toolbar.hpp"

#include "util/time.hpp"

using namespace inferonix;
using namespace inferonix::window;
using namespace inferonix::renderer;
using namespace inferonix::scene;

namespace
{
    auto window_settings_ = window_settings
    {
        .width = 1920,
        .height = 1080,
        .title = "Inferonix Engine",
        .full_screen = false,
        .v_sync = true
    };

    auto log_scene_registry(registry& registry) -> void
    {
        const auto view = registry.view<tag_component>();
        spdlog::info("Scene has following entities: ");
        for (const auto entity : view)
        {
            const auto& tag = view.get<tag_component>(entity).tag;
            auto id = std::string{"no ID"};
            if (registry.any_of<id_component>(entity))
                id = registry.get<id_component>(entity).id;
            spdlog::info("We have entity: {}, with ID: {}", tag, id);

            if (registry.any_of<transform_component>(entity))
            {
                const auto& transform_ = registry.get<transform_component>(entity);
                spdlog::info("Entity has transform: {}, {}, {}",
                    transform_.position.x, transform_.position.y, transform_.position.z
                );
            }
        }
        spdlog::info("");
    }
}

inferonix_engine::inferonix_engine()
    : _window(std::make_shared<window::window>(window_settings_)),
      _renderer(std::make_shared<renderer::renderer>(_window)),
      _scene(std::make_unique<scene::scene>()),
      _scene_serializer(std::make_unique<scene_serializer>(*_scene))
{}

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
    configure_console_sink();

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

void inferonix_engine::configure_console_sink() const
{
    // create a spdlog sink pointing to the dev console
    auto log_callback = [this](const std::string& message) {
        _window->get_layer<ui::dev_console>().insert_log(message);
    };
    auto const custom_sink = std::make_shared<ui::console_sink>(log_callback);
    custom_sink->set_pattern("[%Y-%m-%d %T] [%l] %v");
    spdlog::default_logger()->sinks().push_back(custom_sink);
}


void inferonix_engine::add_window_layers()
{
    _window->add_layer<ui::dockspace>();
    _window->add_layer<ui::scene_layer>(_renderer->get_frame_buffer());
    _window->add_layer<ui::dev_console>(
        [this](const std::string_view command) -> void { _command_registry.execute(command); }
    );
    _window->add_layer<ui::toolbar>(
        [this]() -> void { _state = EngineState::PLAY;  },
        [this]() -> void { _state = EngineState::PAUSE; }
    );

}

void inferonix_engine::register_commands()
{
    _command_registry.register_command(
            "quit", "Exits the engine", [this](const auto& args)-> void { _window->close(); }
    );

    _command_registry.register_command(
        "wireframe", "wireframe_mode", [this](const auto& args)-> void
        {
            if (args.empty())
            {
                _renderer->toggle_wireframe_mode();
                spdlog::info("Wireframe toggled");
                return;
            }
            const std::string& arg = args[0];
            bool enable = false;
            if (arg == "1" || arg == "true" || arg == "on")
                enable = true;
            else if (arg == "0" || arg == "false" || arg == "off")
                enable = false;
            else
            {
                spdlog::error("Invalid");
                return;
            }
            _renderer->set_wireframe_mode(enable);
            spdlog::info("Wireframe mode set to {}", enable ? "ON" : "OFF");

        }
    );
}


