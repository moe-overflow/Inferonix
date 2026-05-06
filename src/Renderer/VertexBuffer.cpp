#include "Buffer.hpp"

using namespace Inferonix::Renderer;

VertexBuffer::VertexBuffer(size_t const size) : _size(size)
{}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::BufferData(std::vector<Vertex> const& vertices)
{
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}
