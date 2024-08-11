#pragma once

#include "shader_program.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "GLFW/glfw3.h"

#include <cmath>
#include <utility>
#include <vector>

namespace Engine::Renderer
{
    struct render_entity_data
    {
        std::vector<float> vertices;
        std::vector<int> indices;
    };

    struct render_entity
    {
        render_entity(  std::unique_ptr<shader_program> program,
                    std::unique_ptr<vertex_array> array,
                    std::unique_ptr<vertex_buffer> vertex_buffer,
                    std::unique_ptr<index_buffer> index_buffer ) :
            ShaderProgram(std::move(program)),
            VertexArray(std::move(array)),
            VertexBuffer(std::move(vertex_buffer)),
            IndexBuffer(std::move(index_buffer)) 
        {}


        explicit render_entity(const render_entity_data& data)
                : ShaderProgram(std::make_unique<shader_program>()),
                  VertexArray(std::make_unique<vertex_array>()),
                  VertexBuffer(std::make_unique<vertex_buffer>(data.vertices.size() * sizeof(float), data.vertices.data())),
                  IndexBuffer(std::make_unique<index_buffer>(data.indices.size() * sizeof(int), data.indices.data())) 
        {}

        // Prevent copying
        render_entity(const render_entity&) = delete;

        explicit render_entity(std::unique_ptr<render_entity> entity) // todo: rewrite?
                : ShaderProgram(std::move(entity->ShaderProgram)),
                  VertexArray(std::move(entity->VertexArray)),
                  VertexBuffer(std::move(entity->VertexBuffer)),
                  IndexBuffer(std::move(entity->IndexBuffer))
        {}

        render_entity& operator=(const render_entity&) = delete;

        // Enable moving
        render_entity(render_entity&& other) noexcept
                : ShaderProgram(std::move(other.ShaderProgram)),
                  VertexArray(std::move(other.VertexArray)),
                  VertexBuffer(std::move(other.VertexBuffer)),
                  IndexBuffer(std::move(other.IndexBuffer))
        {}

        render_entity& operator=(render_entity&& other) noexcept {
            if (this != &other) {
                ShaderProgram = std::move(other.ShaderProgram);
                VertexArray = std::move(other.VertexArray);
                VertexBuffer = std::move(other.VertexBuffer);
                IndexBuffer = std::move(other.IndexBuffer);
            }
            return *this;
        }

        std::unique_ptr<shader_program> ShaderProgram;
        std::unique_ptr<vertex_array> VertexArray;
        std::unique_ptr<vertex_buffer> VertexBuffer;
        std::unique_ptr<index_buffer> IndexBuffer;

    };


    class renderer
    {
    public:
        renderer();
        ~renderer() = default;
        void render();
        void set_render_entity(const std::vector<render_entity_data>& data);
        static void set_clear_color(float r, float g, float b, float a);
        static void clear();
        static void log_info();
        static std::unique_ptr<render_entity> create_render_entity(const render_entity_data& data);

    private:
        std::unique_ptr<std::vector<render_entity>> _render_entities;

    };
}
