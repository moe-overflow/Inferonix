#pragma once


#include <glad/glad.h>
#include <GL/gl.h>
#include <algorithm>
#include <memory>

namespace Inferonix::Renderer
{

    class vertex_array
    {

    public:
        vertex_array();
        //~index_buffer() override;

        vertex_array(vertex_array&& other) noexcept : _id(std::move(other._id)) { other._id.reset(); };
        vertex_array& operator=(vertex_array&& other) noexcept {
            if (this != &other) { // Self-assignment check
                _id = std::move(other._id);
                other._id.reset();
            }
            return *this;
        }

        void bind() const;
        void unbind() const;

        void add_vertex_buffer(const vertex_buffer &vertex_buffer, const vertex_buffer_layout& layout) const;
        void set_index_buffer(const index_buffer& index_buffer) const;

    private:
        std::unique_ptr<uint32_t> _id;

    };

}