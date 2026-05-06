#include "buffer.hpp"

#include <spdlog/spdlog.h>

using namespace Inferonix::Renderer;

void index_buffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void index_buffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void index_buffer::BufferData(std::vector<uint32_t>& indices)
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
