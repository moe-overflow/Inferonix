#include "InferonixEngine.hpp"

#include "Utility/UUID.hpp"
#include "Scene/Components.hpp"

using namespace Inferonix;
using namespace Inferonix::Window;
using namespace Inferonix::Renderer;
using namespace Inferonix::Scene;

WindowSettings workbench_window_settings{ .width = 1920,
                                          .height = 1080,
                                          .title = "Inferonix Engine",
                                          .full_screen = false,
                                          .v_sync = true };

InferonixEngine::InferonixEngine()
    : _window(std::make_shared<Window::Window>(workbench_window_settings)),
      _renderer(std::make_shared<Renderer::Renderer>(_window)),
      _scene(std::make_unique<Scene::Scene>())
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
    _initialized = true;
}

InferonixEngine::~InferonixEngine()
{
    if (_initialized)
    {
        _script_launcher.Terminate();
    }
}

void InferonixEngine::run()
{
    while (!_window->ShouldClose())
    {
        float const delta_time = _window->GetDeltaTime();
        _window->PollEvents();


        if (_initialized)
        {
            _script_launcher.Start(_scene->GetRegistry());
            _script_launcher.Update(_scene->GetRegistry(), delta_time);
        }

        _renderer->Clear();
        _renderer->Render(*_scene);

        _window->SwapBuffers();
    }
}

