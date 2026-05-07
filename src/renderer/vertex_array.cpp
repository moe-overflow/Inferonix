#include "vertex_array.hpp"
#include "buffer.hpp"
#include "spdlog/spdlog.h"

using namespace inferonix::renderer;

vertex_array::vertex_array() : _id(0)
{
    glGenVertexArrays(1, &_id);
}

void vertex_array::bind() const
{
    glBindVertexArray(_id);
}

void vertex_array::unbind() const
{
    glBindVertexArray(0);
}

void vertex_array::add_vertex_buffer(vertex_buffer const& vertex_buffer, vertex_buffer_layout const& layout) const
{
    this->bind();
    vertex_buffer.bind();

    auto const& elements = layout.get_elements();
    uint32_t offset = 0;

    for (uint32_t i = 0; i < elements.size(); i++)
    {
        auto const& [type, size, normalized] = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
                i,
                static_cast<GLint>(size),
                vertex_buffer_element::to_gl_type(type),
                normalized ? GL_TRUE : GL_FALSE,
                static_cast<GLsizei>(layout.get_stride()),
                reinterpret_cast<void const*>(offset)
        );
        offset += size * vertex_buffer_element::get_type_size(type);
    }
}

void vertex_array::set_index_buffer(index_buffer const& index_buffer) const
{
    this->bind();
    index_buffer.bind();
}
