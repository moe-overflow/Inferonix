#include "application.hpp"
#include "main.hpp"
#include "UI/Panels/main_panel.hpp"

using namespace Inferonix;
using namespace Window;
using namespace Renderer;
using namespace UI;

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
    _ui_manager(std::make_shared<ui_manager>(_window))
{
    _ui_manager->init();
}


void application::run()
{
    _ui_manager->add_panel(std::make_unique<UI::main_panel>());
    while (!_window->should_close())
    {
        _window->poll_events();
        _window->swap_buffers();

        _renderer->clear();
        _renderer->render();

        _ui_manager->display_ui();

    }

    _ui_manager->terminate();
}
