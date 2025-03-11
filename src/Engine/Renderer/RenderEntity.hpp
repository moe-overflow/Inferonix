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

namespace Inferonix::Renderer
{
    struct RenderEntityData
    {
        std::vector<float> Vertices;
        std::vector<int> Indices;
        Transform Transform{};
        bool DynamicallyColored;
        // bool PolygonMode;

        virtual ~RenderEntityData() = default;

        virtual void Update(float delta_time) = 0;
    };

    struct RenderEntity
    {
        std::unique_ptr<ShaderProgram> ShaderProgram;
        std::unique_ptr<VertexArray> VertexArray;
        std::unique_ptr<VertexBuffer> VertexBuffer;
        std::unique_ptr<IndexBuffer> IndexBuffer;

        std::shared_ptr<RenderEntityData> RenderEntityData;


        RenderEntity() = default;
        ~RenderEntity() = default;

        RenderEntity(RenderEntity const&) = delete;
        RenderEntity& operator=(RenderEntity const&) = delete;

        RenderEntity(RenderEntity&& other) noexcept
            : ShaderProgram(std::move(other.ShaderProgram)),
              VertexArray(std::move(other.VertexArray)),
              VertexBuffer(std::move(other.VertexBuffer)),
              IndexBuffer(std::move(other.IndexBuffer)),
              RenderEntityData(std::move(other.RenderEntityData))
        {
        }

        RenderEntity& operator=(RenderEntity&& other) noexcept
        {
            if (this != &other)
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
} // namespace Inferonix::Renderer
