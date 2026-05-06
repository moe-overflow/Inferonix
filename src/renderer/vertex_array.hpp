#pragma once

#include "buffer.hpp"

namespace Inferonix::Renderer
{

    class vertex_array
    {

    public:
        vertex_array();
        ~vertex_array() = default;

        void Bind() const;
        void Unbind() const;

        void AddVertexBuffer(vertex_buffer const& vertex_buffer, VertexBufferLayout const& layout) const;
        void SetIndexBuffer(index_buffer const& index_buffer) const;

    private:
        uint32_t _id;
    };

}
