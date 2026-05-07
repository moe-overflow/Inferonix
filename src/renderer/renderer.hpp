#pragma once

#include "spdlog/spdlog.h"

#include "../scene/camera.hpp"
#include "../window/window.hpp"
#include "shader_program.hpp"
#include "vertex_array.hpp"
#include "buffer.hpp"
#include "../scene/components.hpp"
#include "../scene/scene.hpp"

namespace inferonix::renderer
{
    struct graphics_profile
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

    class renderer final : public events::event_listener
    {
    public:
        explicit renderer(std::shared_ptr<window::window> window);

    private:
        static void setup_opengl_debug();

    public:

        renderer(renderer const&) = delete;
        renderer(renderer&&) = delete;

        renderer& operator=(renderer const&) = delete;
        renderer& operator=(renderer&&) = delete;

        ~renderer() override = default;

        void render(scene::scene& scene);

    private:
        void create_render_entity(scene::entity const& entity, scene::mesh_component& mesh);

    public:
        static void set_clear_color(float r, float g, float b, float a);

        static void clear();

        void set_device_specs();

        [[nodiscard]] std::shared_ptr<scene::camera> get_camera()
        {
            return _main_camera;
        }

        void set_camera(std::shared_ptr<scene::camera> const& camera)
        {
            _main_camera = camera;
        }

        void on_event(events::event& event) override;


    private:
        std::vector<std::unique_ptr<RenderEntity>> _render_entities;

        std::shared_ptr<scene::camera> _main_camera;
        std::shared_ptr<window::window> _window_instance{};

        bool _wireframe_mode{ false };

        graphics_profile _device_specs;


        std::unique_ptr<RenderEntity> _grid;
        void setup_grid();


    };
}
