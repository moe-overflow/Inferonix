#pragma once

#include "shader_program.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "GLFW/glfw3.h"

#include <cmath>
#include <utility>
#include <vector>

#include "transform.hpp"
#include <spdlog/spdlog.h>

#include "../Scene/camera.hpp"
#include "../Window/window.hpp"
#include "render_entity.hpp"

namespace Inferonix::Renderer
{

    class renderer
    {
    public:
        explicit renderer(std::shared_ptr<Window::window> window);

        renderer(const renderer&) = delete;
        renderer(renderer&&) = delete;

        renderer& operator=(const renderer&) = delete;
        renderer& operator=(renderer&&) = delete;

        ~renderer() = default;

        void render();
        void add_render_entity(const std::shared_ptr<render_entity_data>& data);
        static void set_clear_color(float r, float g, float b, float a);
        static void clear();
        static void log_info();
        static std::shared_ptr<render_entity> create_render_entity(std::shared_ptr<render_entity_data>);

        [[nodiscard]] std::shared_ptr<Scene::camera> get_camera() { return _main_camera; }
        void set_camera(const std::shared_ptr<Scene::camera>& camera) { _main_camera = camera; }

    private:
        std::vector<std::shared_ptr<render_entity>> _render_entities;
        std::shared_ptr<Scene::camera> _main_camera;

        std::shared_ptr<Window::window> _window_instance{};
    };
}
