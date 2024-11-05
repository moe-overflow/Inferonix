#include "application.hpp"
#include "main.hpp"

using namespace Inferonix::Interface;
using namespace Inferonix::Renderer;

window_settings editor_window_settings{
    .width = 1920,
    .height = 1080,
    .title = "Inferonix Engine",
    .full_screen = false,
    .v_sync = false
};

application::application() :
    _window(std::make_shared<window>(editor_window_settings) ),
    _renderer(std::make_shared<renderer>(_window))
{}


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
