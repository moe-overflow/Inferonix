#include "inferonix_engine.hpp"

#include "util/uuid.hpp"
#include "scene/components.hpp"
#include "renderer/renderer.hpp"

using namespace Inferonix;
using namespace Inferonix::Window;
using namespace Inferonix::Renderer;
using namespace Inferonix::Scene;

auto workbench_window_settings =  WindowSettings { .width = 1920,
                                          .height = 1080,
                                          .title = "Inferonix Engine",
                                          .full_screen = false,
                                          .v_sync = true };

inferonix_engine::inferonix_engine()
    : _window(std::make_shared<window>(workbench_window_settings)),
      _renderer(std::make_shared<renderer>(_window)),
      _scene(std::make_unique<scene>())
{

/*

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
*/

    _script_launcher.Launch();

    {

        auto mesh_ = _scene->GetAssetRegistry().Load<mesh>(
            UUID::Generate(),
            RESOURCES_PATH "models/Monkey.obj"
        );

        // 1. Create a Static Floor (Invisible, just catches the monkey)
        auto floor = _scene->GetRegistry().create();
        _scene->GetRegistry().emplace<TransformComponent>(floor).position = {0, -5.0f, 0};
        _scene->GetRegistry().emplace<BoxColliderComponent>(floor).HalfExtents = {50.0f, 1.0f, 50.0f};
        _scene->GetRegistry().emplace<RigidBodyComponent>(floor, 0.0f, JPH::BodyID(), RigidBodyType::Static);

        // 2. Create the Falling Monkey!
        auto box = _scene->GetRegistry().create();
        _scene->GetRegistry().emplace<TransformComponent>(box).position = {0, 10.0f, 0}; // Drop from high up!
        _scene->GetRegistry().emplace<BoxColliderComponent>(box).HalfExtents = {0.5f, 0.5f, 0.5f};
        _scene->GetRegistry().emplace<RigidBodyComponent>(box, 1.0f, JPH::BodyID(), RigidBodyType::Dynamic);

        if (mesh_.has_value())
            _scene->GetRegistry().emplace<MeshComponent>(box, *mesh_.value());

    }
    _initialized = true;
}

inferonix_engine::~inferonix_engine()
{
    if (_initialized)
    {
        _script_launcher.Terminate();
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

void inferonix_engine::Run()
{
    _physics_engine.Init();
    _physics_engine.StartSimulation(_scene->GetRegistry());


    while (!_window->ShouldClose())
    {
        float const delta_time = _window->GetDeltaTime();

        window::PollEvents();

        if (_initialized)
        {
            _script_launcher.Start(_scene->GetRegistry());
            _script_launcher.Update(_scene->GetRegistry(), delta_time);
            _physics_engine.Update(_scene->GetRegistry(), delta_time);
        }

        _scene->GetEditorCamera()->Update(delta_time);

        renderer::Clear();
        _renderer->Render(*_scene);

        _window->SwapBuffers();
    }
}

