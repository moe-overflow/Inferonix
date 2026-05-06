#include "vertex_array.hpp"
#include "buffer.hpp"
#include "spdlog/spdlog.h"

using namespace Inferonix::Renderer;

vertex_array::vertex_array() : _id(0)
{
    glGenVertexArrays(1, &_id);
}

void vertex_array::Bind() const
{
    glBindVertexArray(_id);
}

void vertex_array::Unbind() const
{
    glBindVertexArray(0);
}

void vertex_array::AddVertexBuffer(vertex_buffer const& vertex_buffer, VertexBufferLayout const& layout) const
{
    this->Bind();
    vertex_buffer.Bind();

    auto const& elements = layout.GetElements();
    uint32_t offset = 0;

    for (uint32_t i = 0; i < elements.size(); i++)
    {
        auto const& [type, size, normalized] = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
                i,
                static_cast<GLint>(size),
                VertexBufferElement::ToGlType(type),
                normalized ? GL_TRUE : GL_FALSE,
                static_cast<GLsizei>(layout.GetStride()),
                reinterpret_cast<void const*>(offset)
        );
        offset += size * VertexBufferElement::GetTypeSize(type);
    }
}

void vertex_array::SetIndexBuffer(index_buffer const& index_buffer) const
{
    this->Bind();
    index_buffer.Bind();
}
