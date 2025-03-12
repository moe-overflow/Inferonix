#include "Buffer.hpp"

using namespace Inferonix::Renderer;


IndexBuffer::IndexBuffer(int64_t count, void const* indices) : Buffer(), _count(count) { }

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::BufferData(int64_t count, void const* indices)
{
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(count * sizeof(unsigned int)),
            indices,
            GL_STATIC_DRAW
    );
}
