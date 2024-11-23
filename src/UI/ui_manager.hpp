#pragma once

#include "ui_manager.hpp"

#include <utility>

#include "../Window/window.hpp"
#include "Panels/panel.hpp"

namespace Inferonix::UI
{
    class ui_manager
    {
    public:
        explicit ui_manager(std::shared_ptr<Window::window> window) :
            _window(std::move(window))
        {}

        ~ui_manager() = default;

        void init();

        void display_ui();

    private:

        void begin_frame();

        void end_frame();

        void render_panels();

    public:
        void terminate();

        void add_panel(std::unique_ptr<panel> panel);

    private:
        std::shared_ptr<Window::window> _window;

        std::vector<std::unique_ptr<panel>> _panels;

    };
}

