#pragma once

#include "../Window/window.hpp"
#include "../Renderer/renderer.hpp"
#include "../UI/ui_manager.hpp"

#include <memory>


namespace Inferonix::Interface
{

    class application
    {
    public:
        application();

        application(application const&) = delete;

        application(application&&) = delete;

        application &operator=(const application&) = delete;

        application &operator=(application&&) = delete;

        ~application() = default;

        void run();

    protected:
        std::shared_ptr<Window::window> _window;
        std::shared_ptr<Renderer::renderer> _renderer;
        std::shared_ptr<UI::ui_manager> _ui_manager;

    };

}

extern Inferonix::Interface::application *create_app();
