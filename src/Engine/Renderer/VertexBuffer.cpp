#include "Buffer.hpp"

using namespace Inferonix::Renderer;

VertexBuffer::VertexBuffer(size_t const size) : Buffer(), _size(size) { }

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::BufferData(size_t const size, void const* positions)
{
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), positions, GL_STATIC_DRAW);
}
