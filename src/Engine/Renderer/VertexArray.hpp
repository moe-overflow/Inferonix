#pragma once

#include "Buffer.hpp"
#include "glad/glad.h"
#include <algorithm>
#include <gl/GL.h>
#include <memory>

namespace Inferonix::Renderer
{

    class VertexArray
    {

    public:
        VertexArray();
        ~VertexArray() = default;

        void Bind() const;
        void Unbind() const;

        void AddVertexBuffer(VertexBuffer const& vertex_buffer, VertexBufferLayout const& layout) const;
        void SetIndexBuffer(IndexBuffer const& index_buffer) const;

    private:
        uint32_t _id;
    };

} // namespace Inferonix::Renderer
