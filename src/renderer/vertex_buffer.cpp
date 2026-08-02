#include "buffer.hpp"

using namespace inferonix::renderer;

vertex_buffer::vertex_buffer(size_t const size) : _size(size)
{}

void vertex_buffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void vertex_buffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void vertex_buffer::buffer_data(std::span<const vertex> vertices)
{
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size_bytes()), vertices.data(), GL_STATIC_DRAW);
    _size = vertices.size();
}
