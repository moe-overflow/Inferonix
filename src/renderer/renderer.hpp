#pragma once

#include "spdlog/spdlog.h"

#include "../scene/camera.hpp"
#include "../window/window.hpp"
#include "shader_program.hpp"
#include "vertex_array.hpp"
#include "buffer.hpp"
#include "../scene/components.hpp"
#include "../scene/scene.hpp"

namespace Inferonix::Renderer
{
    struct GraphicsProfile
    {
        std::string vendor = reinterpret_cast<char const*>(glGetString(GL_VENDOR));
        std::string renderer;
        std::string version;
        std::string shading_language_version;
        int nr_attributes;
    };

    struct RenderEntity
    {
        shader_program shader_program_;
        vertex_array vertex_array_;
        vertex_buffer vertex_buffer_;
        index_buffer index_buffer_;
    };

    class renderer final : public EventSystem::event_listener
    {
    public:
        explicit renderer(std::shared_ptr<Window::window> window);

    private:
        static void SetupOpenGLDebug();

    public:

        renderer(renderer const&) = delete;
        renderer(renderer&&) = delete;

        renderer& operator=(renderer const&) = delete;
        renderer& operator=(renderer&&) = delete;

        ~renderer() override = default;

        void Render(Scene::scene& scene);

    private:
        void CreateRenderEntity(Scene::Entity const& entity, Scene::MeshComponent& mesh);

    public:
        static void SetClearColor(float r, float g, float b, float a);

        static void Clear();

        void SetDeviceSpecs();

        [[nodiscard]] std::shared_ptr<Scene::camera> GetCamera()
        {
            return _main_camera;
        }

        void SetCamera(std::shared_ptr<Scene::camera> const& camera)
        {
            _main_camera = camera;
        }

        void OnEvent(EventSystem::event& event) override;


    private:
        std::vector<std::unique_ptr<RenderEntity>> _render_entities;

        std::shared_ptr<Scene::camera> _main_camera;
        std::shared_ptr<Window::window> _window_instance{};

        bool _wireframe_mode{ false };

        GraphicsProfile _device_specs;


        std::unique_ptr<RenderEntity> _grid;
        void SetupGrid();


    };
}
