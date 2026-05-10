#include "inferonix_engine.hpp"

#include "scene/components.hpp"
#include "renderer/renderer.hpp"
#include "scene/scene_serializer.hpp"

#include "ui/layer.hpp"
#include "ui/dev_console.hpp"
#include "ui/console_sink.hpp"

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
{

#if 0

    // _renderer->SetCamera(_scene->GetMainCamera());
    EventSystem::EventHandler::Get()->Subscribe(_renderer);

    // Create a simple test scene
    auto& asset_registry = _scene->GetAssetRegistry();
    auto& registry = _scene->GetRegistry();

    auto mesh = asset_registry.Load<Mesh>(
        UUID::Generate(),
        RESOURCES_PATH "models/Monkey.obj"
    );

    if (mesh.has_value())
    {
        auto const entity = registry.create();
        registry.emplace<MeshComponent>(entity, *mesh.value());

        TransformComponent transform{};
        transform.position = {0.0f, 0.0f, -5.0f};
        transform.scale = {0.5f, 0.5f, 0.5f};
        registry.emplace<TransformComponent>(entity, transform);

        // Add a script component to this entity
        ScriptComponent script_comp{};
        script_comp.script_path = RESOURCES_PATH "scripts/Player.as";
        script_comp.initialized = false;
        registry.emplace<ScriptComponent>(entity, std::move(script_comp));

        spdlog::info("Created test entity with Triangle mesh and Player script");
    }
    else
    {
        spdlog::warn("Failed to load mesh, creating entity without mesh");
        auto const entity = registry.create();
        TransformComponent transform{};
        transform.position = {0.0f, 0.0f, -5.0f};
        registry.emplace<TransformComponent>(entity, transform);
    }
#endif

#if 0
    {

        auto mesh_ = _scene->get_asset_registry().load<mesh>(
            utils::generate_uuid(),
            RESOURCES_PATH "models/Monkey.obj"
        );

        // 1. Create a Static Floor (Invisible, just catches the monkey)
        auto floor = _scene->get_registry().create();
        _scene->get_registry().emplace<transform_component>(floor).position = {0, -5.0f, 0};
        _scene->get_registry().emplace<box_collider_component>(floor).half_extents = {50.0f, 1.0f, 50.0f};
        _scene->get_registry().emplace<rigid_body_component>(floor, 0.0f, JPH::BodyID(), RigidBodyType::Static);

        // 2. Create the Falling Monkey!
        auto box = _scene->get_registry().create();
        _scene->get_registry().emplace<transform_component>(box).position = {0, 10.0f, 0}; // Drop from high up!
        _scene->get_registry().emplace<box_collider_component>(box).half_extents = {0.5f, 0.5f, 0.5f};
        _scene->get_registry().emplace<box_collider_component>(box, 1.0f, JPH::BodyID(), RigidBodyType::Dynamic);

        if (mesh_.has_value())
            _scene->get_registry().emplace<mesh_component>(box, *mesh_.value());

    }
#endif
}

inferonix_engine::~inferonix_engine()
{
    if (_initialized)
    {
        _script_launcher.terminate();
    }
}

/*
const float FIXED_DT = 1.0f / 60.0f; // Exactly 60Hz
float accumulator = 0.0f;

while (!_window->ShouldClose()) {
    float frame_time = _window->GetDeltaTime();
    accumulator += frame_time;

    // Run physics/logic in exact chunks
    while (accumulator >= FIXED_DT) {
        _physics_engine.StepSimulation(FIXED_DT);
        _script_launcher.FixedUpdate(_scene->GetRegistry(), FIXED_DT);
        accumulator -= FIXED_DT;
    }

    // Render as fast as possible
    _renderer->Render(*_scene);
}
 */


void inferonix_engine::init(const std::string_view scene_path)
{

    _window->add_layer<ui::dockspace>();
    _window->add_layer<ui::scene_layer>(_renderer->get_frame_buffer());
    _window->add_layer<ui::dev_console>();

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
        float const delta_time = _window->get_delta_time();

        window::window::poll_events();

        if (_initialized)
        {
            _script_launcher.start(_scene->get_registry());
            _script_launcher.update(_scene->get_registry(), delta_time);
            _physics_engine.update(_scene->get_registry(), delta_time);
        }

        _scene->get_editor_camera()->update(delta_time);

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

