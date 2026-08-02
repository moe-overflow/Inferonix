#pragma once

#include "buffer.hpp"

namespace inferonix::renderer
{

    class vertex_array final
    {

    public:
        vertex_array();
        ~vertex_array() = default;

        void bind() const;
        void unbind() const;

        void add_vertex_buffer(vertex_buffer const& vertex_buffer, vertex_buffer_layout const& layout) const;
        void set_index_buffer(index_buffer const& index_buffer) const;

    private:
        uint32_t _id;
    };

}
