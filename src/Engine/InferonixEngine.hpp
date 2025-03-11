#pragma once

#include <memory>

#include "Project/Project.hpp"
#include "Renderer/Renderer.hpp"
#include "Window/Window.hpp"

namespace Inferonix
{

    class InferonixEngine
    {
    public:
        InferonixEngine();

        InferonixEngine(InferonixEngine const&) = delete;

        InferonixEngine(InferonixEngine&&) = delete;

        InferonixEngine& operator=(InferonixEngine const&) = delete;

        InferonixEngine& operator=(InferonixEngine&&) = delete;

        ~InferonixEngine() = default;

        void run();

    protected:
        std::shared_ptr<Window::Window> _window;
        std::shared_ptr<Renderer::Renderer> _renderer;
        std::unique_ptr<Project::Project> _project;
    };

} // namespace Inferonix
