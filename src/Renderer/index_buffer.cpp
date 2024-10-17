#include "buffer.hpp"

using namespace Inferonix::Renderer;


index_buffer::index_buffer(size_t count, const void* indices)
    : buffer(), _count(count)
{}

void index_buffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void index_buffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void index_buffer::buffer_data(size_t count, const void * indices)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count), indices, GL_STATIC_DRAW);
}

