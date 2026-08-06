#include "buffer.hpp"

#include <spdlog/spdlog.h>

using namespace inferonix::renderer;

void index_buffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void index_buffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void index_buffer::buffer_data(const std::vector<uint32_t>& indices)
{
    _count = indices.size();
    LOG(LOG_TYPE::INFO, "Buffering data (count = {}) for index buffer {}", _id, _count);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(uint32_t),
            indices.data(),
            GL_STATIC_DRAW
    );
}
