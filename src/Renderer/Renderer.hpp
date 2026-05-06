#pragma once

#include "spdlog/spdlog.h"

#include "../Scene/Camera.hpp"
#include "../Window/Window.hpp"
#include "ShaderProgram.hpp"
#include "VertexArray.hpp"
#include "Buffer.hpp"
#include "../Scene/Components.hpp"
#include "../Scene/Scene.hpp"

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
        ShaderProgram shader_program;
        VertexArray vertex_array;
        VertexBuffer vertex_buffer;
        IndexBuffer index_buffer;
    };

    class Renderer final : public EventSystem::EventListener
    {
    public:
        explicit Renderer(std::shared_ptr<Window::Window> window);

    private:
        static void SetupOpenGLDebug();

    public:

        Renderer(Renderer const&) = delete;
        Renderer(Renderer&&) = delete;

        Renderer& operator=(Renderer const&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        ~Renderer() override = default;

        void Render(Scene::Scene& scene);

    private:
        void CreateRenderEntity(Scene::Entity const& entity, Scene::MeshComponent& mesh);

    public:
        static void SetClearColor(float r, float g, float b, float a);

        static void Clear();

        void SetDeviceSpecs();

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
        std::vector<std::unique_ptr<RenderEntity>> _render_entities;

        std::shared_ptr<Scene::Camera> _main_camera;
        std::shared_ptr<Window::Window> _window_instance{};

        bool _wireframe_mode{ false };

        GraphicsProfile _device_specs;


        std::unique_ptr<RenderEntity> _grid;
        void SetupGrid();


    };
}
