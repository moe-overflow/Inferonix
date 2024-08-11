#include "buffer.hpp"

using namespace Inferonix::Renderer;

vertex_buffer::vertex_buffer(size_t size, const void *vertex_data) : buffer(), _size(size)  { }

vertex_buffer::vertex_buffer(const vertex_buffer &other)  :
    buffer(), _size(other._size)
{
    bind();
    buffer_data(other._size, nullptr);
}

vertex_buffer &vertex_buffer::operator=(const vertex_buffer &other)
{
    if (this != &other)
    {
        glDeleteBuffers(1, _id.get());
        _size = other._size;
        glGenBuffers(1, _id.get());
        bind();
        glBufferData(GL_ARRAY_BUFFER, _size, nullptr, GL_STATIC_DRAW);
    }
    return *this;
}

void vertex_buffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, *_id);
}

void vertex_buffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vertex_buffer::buffer_data(size_t size, const void *positions) const
{
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), positions, GL_STATIC_DRAW);
}

