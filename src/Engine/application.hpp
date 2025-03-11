#pragma once

#include <memory>

#include "Window/window.hpp"
#include "Renderer/renderer.hpp"
#include "Project/project.hpp"

namespace Inferonix
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
        std::unique_ptr<Project::project> _project;
    };

}
