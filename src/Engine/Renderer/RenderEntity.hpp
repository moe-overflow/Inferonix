#pragma once

#include "Buffer.hpp"
#include "ShaderProgram.hpp"
#include "VertexArray.hpp"
#include <utility>
#include <vector>

#include "Transform.hpp"
#include "spdlog/spdlog.h"

#include "../Window/Window.hpp"
#include "Mesh.hpp"

namespace Inferonix::Renderer
{
    struct RenderEntityData : public EventSystem::EventListener
    {
        std::shared_ptr<Mesh> mesh_instance;
        Transform transform{};
        bool dynamically_colored;

        ~RenderEntityData() override = default;

        virtual void Update(float delta_time) = 0;
    };

    struct RenderEntity
    {
        std::unique_ptr<ShaderProgram> shaderProgram;
        std::unique_ptr<VertexArray> vertexArray;
        std::unique_ptr<VertexBuffer> vertexBuffer;
        std::unique_ptr<IndexBuffer> indexBuffer;

        std::shared_ptr<RenderEntityData> renderEntityData;


        RenderEntity() = default;
        ~RenderEntity() = default;

        RenderEntity(RenderEntity const&) = delete;
        RenderEntity& operator=(RenderEntity const&) = delete;

        RenderEntity(RenderEntity&& other) noexcept
            : shaderProgram(std::move(other.shaderProgram)),
              vertexArray(std::move(other.vertexArray)),
              vertexBuffer(std::move(other.vertexBuffer)),
              indexBuffer(std::move(other.indexBuffer)),
              renderEntityData(std::move(other.renderEntityData))
        {
        }

        RenderEntity& operator=(RenderEntity&& other) noexcept
        {
            if (this != &other)
            {
                using std::swap;
                swap(shaderProgram, other.shaderProgram);
                swap(vertexArray, other.vertexArray);
                swap(vertexBuffer, other.vertexBuffer);
                swap(indexBuffer, other.indexBuffer);
                swap(renderEntityData, other.renderEntityData);
            }
            return *this;
        }
    };
} // namespace Inferonix::Renderer
