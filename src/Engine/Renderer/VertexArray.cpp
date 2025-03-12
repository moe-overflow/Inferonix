#include "VertexArray.hpp"
#include "Buffer.hpp"
#include "spdlog/spdlog.h"

using namespace Inferonix::Renderer;

VertexArray::VertexArray() : _id(0)
{
    glGenVertexArrays(1, &_id);
}

void VertexArray::Bind() const
{
    glBindVertexArray(_id);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(VertexBuffer const& vertex_buffer, VertexBufferLayout const& layout) const
{
    this->Bind();
    vertex_buffer.Bind();

    auto const& elements = layout.GetElements();
    uint32_t offset = 0;

    for (uint32_t i = 0; i < elements.size(); i++)
    {
        auto const& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
                i,
                static_cast<GLint>(element.size),
                VertexBufferElement::ToGlType(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                static_cast<GLsizei>(layout.GetStride()),
                reinterpret_cast<void const*>(offset)
        );
        offset += element.size * VertexBufferElement::GetTypeSize(element.type);
    }
}

void VertexArray::SetIndexBuffer(IndexBuffer const& index_buffer) const
{
    this->Bind();
    index_buffer.Bind();
}
