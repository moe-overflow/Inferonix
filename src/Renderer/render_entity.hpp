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
#include "Interface/window.hpp"

namespace Inferonix::Renderer
{
    struct render_entity_data
    {
        std::vector<float> vertices;
        std::vector<int> indices;
        transform transform{};
        bool dynamically_colored;
        // bool polygon_mode;

        virtual ~render_entity_data() = default;

        virtual void update(float delta_time) = 0;

    };

    struct render_entity
    {
        std::unique_ptr<shader_program> ShaderProgram;
        std::unique_ptr<vertex_array> VertexArray;
        std::unique_ptr<vertex_buffer> VertexBuffer;
        std::unique_ptr<index_buffer> IndexBuffer;

        std::shared_ptr<render_entity_data> RenderEntityData;


        render_entity() = default;
        ~render_entity() = default;

        render_entity(const render_entity&) = delete;
        render_entity& operator=(const render_entity&) = delete;

        render_entity(render_entity&& other) noexcept :
            ShaderProgram(std::move(other.ShaderProgram)),
            VertexArray(std::move(other.VertexArray)),
            VertexBuffer(std::move(other.VertexBuffer)),
            IndexBuffer(std::move(other.IndexBuffer)),
            RenderEntityData(std::move(other.RenderEntityData))
        {}

        render_entity& operator=(render_entity&& other) noexcept
        {
            if(this != &other)
            {
                using std::swap;
                swap(ShaderProgram, other.ShaderProgram);
                swap(VertexArray, other.VertexArray);
                swap(VertexBuffer, other.VertexBuffer);
                swap(IndexBuffer, other.IndexBuffer);
                swap(RenderEntityData, other.RenderEntityData);
            }
            return *this;
        }
    };
}