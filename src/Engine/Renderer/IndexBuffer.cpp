#include "Buffer.hpp"

using namespace Inferonix::Renderer;


IndexBuffer::IndexBuffer(int64_t const count, void const* indices) : Buffer(), _count(count) { }

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::BufferData(std::vector<uint32_t>& indices)
{
    _count = indices.size();
    spdlog::info("Buffering data (count = {}) for index buffer {}", _id, _count);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(_count * sizeof(unsigned int)),
            reinterpret_cast<GLvoid*>(indices.data()),
            GL_STATIC_DRAW
    );
}
