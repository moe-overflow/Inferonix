#include "application.hpp"
#include "callback_functions.hpp"
#include "main.hpp"

#include <memory>

using namespace Engine::Interface;


application::application() :
    _window(std::make_unique<window>(WIDTH, HEIGHT)), _renderer(std::make_unique<renderer>())
{ }


void application::run()
{
    _renderer->set_clear_color(0.0f, 0.0f, 0.0f, 1.0f);

    while (!_window->should_close())
    {
        _window->poll_events();

        process_input(*_window);

        _renderer->clear();
        _renderer->render();

        _window->swap_buffers();
    }
}
