#include "buffer.hpp"

using namespace Inferonix::Renderer;


index_buffer::index_buffer(size_t count, const void * indices)
    : buffer(), _count(count)
{ }

index_buffer::index_buffer(const index_buffer &other)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *_id);
    buffer_data(other._count, nullptr);
}

index_buffer &index_buffer::operator=(const index_buffer &other)
{
    if (this != &other) {
        glDeleteBuffers(1, _id.get());
        _count = other._count;
        glGenBuffers(1, _id.get());
        bind();
        glBufferData(GL_ARRAY_BUFFER, _count, nullptr, GL_STATIC_DRAW); // Allocate memory
    }
    return *this;
}


void index_buffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *_id);
}

void index_buffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void index_buffer::buffer_data(size_t count, const void * indices)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count), indices, GL_STATIC_DRAW);
}

