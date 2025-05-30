#pragma once

#include <vector>

#include "spdlog/spdlog.h"

#include "../Scene/Camera.hpp"
#include "../Window/Window.hpp"
#include "RenderEntity.hpp"

namespace Inferonix::Renderer
{
    class Renderer final : public EventSystem::EventListener
    {
    public:
        explicit Renderer(std::shared_ptr<Window::Window> window);

        Renderer(Renderer const&) = delete;

        Renderer(Renderer&&) = delete;

        Renderer& operator=(Renderer const&) = delete;

        Renderer& operator=(Renderer&&) = delete;

        ~Renderer() override = default;

        void Render() const;

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

        void OnEvent(EventSystem::Event& event) override;

    private:
        std::vector<std::shared_ptr<RenderEntity>> _render_entities;
        std::shared_ptr<Scene::Camera> _main_camera;
        std::shared_ptr<Window::Window> _window_instance{};

        bool _wireframe_mode{ false };

    };
} // namespace Inferonix::Renderer
