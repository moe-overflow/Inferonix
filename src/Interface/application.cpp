#include "application.hpp"
#include "callback_functions.hpp"
#include "main.hpp"

#include <memory>

using namespace Inferonix::Interface;
using namespace Inferonix::Renderer;

application::application() :
    _window(std::make_unique<window>(WIDTH, HEIGHT)),
    _renderer(std::make_unique<renderer>(_window))
{}


void application::run()
{
    while (!_window->should_close())
    {
        _window->poll_events();

        process_input(*_window);

        _renderer->clear();
        _renderer->render();

        _window->swap_buffers();
    }
}
