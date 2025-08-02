#include "InferonixEngine.hpp"

using namespace Inferonix;
using namespace Inferonix::Window;
using namespace Inferonix::Renderer;

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

    /*
    for (auto const& entity_data : _project->GetEntitiesData())
    {
        _renderer->AddRenderEntity(entity_data);
        EventSystem::EventHandler::Get()->Subscribe(entity_data);
    }
    */
}


void InferonixEngine::run() const
{
    while (!_window->ShouldClose())
    {
        _window->PollEvents();
        _window->SwapBuffers();

        _renderer->Clear();
        _renderer->Render();
    }
}
