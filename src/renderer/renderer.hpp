#pragma once

#include "scene/camera.hpp"
#include "window/window.hpp"
#include "shader_program.hpp"
#include "vertex_array.hpp"
#include "buffer.hpp"
#include "scene/components.hpp"
#include "scene/scene.hpp"


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

    struct gpu_mesh
    {
        vertex_array vertex_array_;
        vertex_buffer vertex_buffer_;
        index_buffer index_buffer_;

        uint32_t material_index = 0;
        std::shared_ptr<texture> albedo;
    };

    struct render_entity
    {
        shader_program shader_program_;
        std::vector<gpu_mesh> meshes;
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

        void setup();

        void toggle_wireframe_mode()
        {
            set_wireframe_mode(!_wireframe_mode);
        }

        void set_wireframe_mode(const bool enable)
        {
            _wireframe_mode = enable;
            glPolygonMode(GL_FRONT_AND_BACK, _wireframe_mode ? GL_LINE : GL_FILL);
        }

    private:
        void create_render_entity(const scene::entity& entity, scene::mesh_component& mesh_component);

    public:
        static void set_clear_color(color& color);

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

        [[nodiscard]] auto get_frame_buffer() -> std::shared_ptr<frame_buffer>
        {
            return _frame_buffer;
        }

        auto set_clear_color(const color& color) -> void
        {
            this->_clear_color = color;
            glClearColor(_clear_color.r, _clear_color.g, _clear_color.b, 1);
        }

    private:
        std::vector<std::unique_ptr<render_entity>> _render_entities;

        std::shared_ptr<scene::camera> _main_camera;
        std::shared_ptr<window::window> _window_instance{};

        bool _wireframe_mode{ false };

        graphics_profile _device_specs{};

        std::unique_ptr<render_entity> _grid;
        void setup_grid();

        std::shared_ptr<frame_buffer> _frame_buffer;

        color _clear_color { colors::black };

    };
}
