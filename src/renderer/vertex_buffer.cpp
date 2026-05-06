#include "buffer.hpp"

using namespace Inferonix::Renderer;

vertex_buffer::vertex_buffer(size_t const size) : _size(size)
{}

void vertex_buffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void vertex_buffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vertex_buffer::BufferData(std::vector<Vertex> const& vertices)
{
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}
