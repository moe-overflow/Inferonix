#include "buffer.hpp"
#include "vertex_array.hpp"

using namespace Inferonix::Renderer;

vertex_array::vertex_array() : _id( std::make_unique<uint32_t>(0))
{
    glGenVertexArrays(1, _id.get());
}

void vertex_array::bind() const
{
    glBindVertexArray(*_id);
}

void vertex_array::unbind() const
{
    glBindVertexArray(0);
}

void vertex_array::add_vertex_buffer(
        const vertex_buffer &vertex_buffer,
        const vertex_buffer_layout& layout) const
{
    this->bind();
    vertex_buffer.bind();

    const auto& elements = layout.get_elements();
    uint32_t offset = 0;

    for(uint32_t i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i,
                              static_cast<GLint>(element.size),
                              vertex_buffer_element::to_gl_type(element.type),
                              element.normalized ? GL_TRUE : GL_FALSE,
                              static_cast<GLsizei>(layout.get_stride()),
                              reinterpret_cast<const void*>(0)
        );
        offset += element.size * vertex_buffer_element::get_type_size(element.type);
    }

}

void vertex_array::set_index_buffer(const index_buffer& index_buffer) const
{
    this->bind();
    index_buffer.bind();
}




