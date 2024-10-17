#pragma once

#include <glad/glad.h>
#include <GL/gl.h>
#include <algorithm>
#include <memory>

namespace Inferonix::Renderer
{

    class vertex_array
    {

    public:
        vertex_array();
        ~vertex_array() = default;

        void bind() const;
        void unbind() const;

        void add_vertex_buffer(const vertex_buffer &vertex_buffer, const vertex_buffer_layout& layout) const;
        void set_index_buffer(const index_buffer& index_buffer) const;

    private:
        uint32_t _id;

    };

}