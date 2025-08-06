#pragma once

#include <memory>

#include "Renderer/Renderer.hpp"
#include "Window/Window.hpp"

namespace Inferonix
{

    class InferonixEngine final
    {
    public:
        InferonixEngine();

        InferonixEngine(InferonixEngine const&) = delete;

        InferonixEngine(InferonixEngine&&) = delete;

        InferonixEngine& operator=(InferonixEngine const&) = delete;

        InferonixEngine& operator=(InferonixEngine&&) = delete;

        ~InferonixEngine() = default;

        void run() const;

    protected:
        std::shared_ptr<Window::Window> _window;
        std::shared_ptr<Renderer::Renderer> _renderer;
        std::unique_ptr<Scene::Scene> _scene;
    };

} // namespace Inferonix
