#pragma once

#include "Buffer.hpp"
#include "GLFW/glfw3.h"
#include "ShaderProgram.hpp"
#include "VertexArray.hpp"

#include <cmath>
#include <utility>
#include <vector>

#include "Transform.hpp"
#include "spdlog/spdlog.h"

#include "../Scene/Camera.hpp"
#include "../Window/Window.hpp"
#include "RenderEntity.hpp"

namespace Inferonix::Renderer
{
    class Renderer
    {
    public:
        explicit Renderer(std::shared_ptr<Window::Window> window);

        Renderer(Renderer const&) = delete;

        Renderer(Renderer&&) = delete;

        Renderer& operator=(Renderer const&) = delete;

        Renderer& operator=(Renderer&&) = delete;

        ~Renderer() = default;

        void Render();

        void AddRenderEntity(std::shared_ptr<RenderEntityData> const& data);

        static void SetClearColor(float r, float g, float b, float a);

        static void Clear();

        static void LogInfo();

        static std::shared_ptr<RenderEntity> CreateRenderEntity(std::shared_ptr<RenderEntityData>);

        [[nodiscard]] std::shared_ptr<Scene::Camera> GetCamera()
        {
            return _main_camera;
        }

        void SetCamera(std::shared_ptr<Scene::Camera> const& camera)
        {
            _main_camera = camera;
        }

    private:
        std::vector<std::shared_ptr<RenderEntity>> _render_entities;
        std::shared_ptr<Scene::Camera> _main_camera;
        std::shared_ptr<Window::Window> _window_instance{};
    };
} // namespace Inferonix::Renderer
