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
                                          .v_sync = false };

InferonixEngine::InferonixEngine()
    : _window(std::make_shared<Window::Window>(workbench_window_settings)),
      _renderer(std::make_shared<Renderer::Renderer>(_window)),
      _scene(std::make_unique<Scene::Scene>())
{
    // _renderer->SetCamera(_scene->GetMainCamera());
    EventSystem::EventHandler::Get()->Subscribe(_renderer);

    // Temporary: Creating hard coded scene for testing ECS
    auto& asset_registry = _scene->GetAssetRegistry();
    auto mesh = asset_registry.Load<Mesh>(
        UUID::Generate(),
        RESOURCES_PATH "models/Monkey.obj"
    );


    auto& registry = _scene->GetRegistry();
    auto const entity = registry.create();
    registry.emplace<MeshComponent>(entity, *mesh.value());

    Transform transform{};
    transform.position = {0.0f, 0.0f, 0.0f};
    registry.emplace<TransformComponent>(entity, transform);

}


void InferonixEngine::run() const
{
    while (!_window->ShouldClose())
    {
        _window->PollEvents();

        _renderer->Clear();
        _renderer->Render(*_scene);

        _window->SwapBuffers();
    }
}
