#include "application.hpp"

using namespace Inferonix;
using namespace Inferonix::Window;
using namespace Inferonix::Renderer;
using namespace Inferonix::Project;

window_settings editor_window_settings{
    .width = 1920,
    .height = 1080,
    .title = "Inferonix Engine",
    .full_screen = false,
    .v_sync = false
};

application::application() :
    _window(std::make_shared<window>(editor_window_settings) ),
    _renderer(std::make_shared<renderer>(_window)),
    _project(std::make_unique<project>())
{
    _renderer->set_camera(_project->get_main_camera());

    for(const auto& entity_data : _project->get_entities_data())
    {
        _renderer->add_render_entity(entity_data);
        EventSystem::event_handler::get()->subscribe(entity_data);
    }
}


void application::run()
{
    while (!_window->should_close())
    {
        _window->poll_events();
        _window->swap_buffers();

        _renderer->clear();
        _renderer->render();

    }
}
