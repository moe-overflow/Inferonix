#pragma once

#include "renderer/buffer.hpp"

#include <cstdint>
#include <utility>
#include <span>

namespace inferonix::renderer
{
    class shader_storage_buffer final
    {
        public:
        shader_storage_buffer()
        {
            glGenBuffers(1, &_id);
        }

        ~shader_storage_buffer()
        {
            if (_id != 0)
                glDeleteBuffers(1, &_id);
        }

        shader_storage_buffer(const shader_storage_buffer&) = delete;
        shader_storage_buffer& operator=(const shader_storage_buffer&) = delete;

        shader_storage_buffer(shader_storage_buffer&& other) noexcept :
            _id{ std::exchange(other._id, 0) },
            _size(std::exchange(other._size, 0))
        {}

        shader_storage_buffer& operator=(shader_storage_buffer&& other) noexcept
        {
            if (this != &other)
            {
                if (_id)
                    glDeleteBuffers(1, &_id);
                _id = std::exchange(other._id, 0);
                _size = std::exchange(other._size, 0);
            }
            return *this;
        }

        auto bind() const -> void
        {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, _id);
        }

        auto static unbind() -> void
        {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        }

        auto bind_base(uint32_t binding_index) const -> void
        {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, _id);
        }

        template<class T>
        auto buffer_data(std::span<const T> data, const GLenum usage = GL_DYNAMIC_DRAW) -> void
        {
            bind();
            glBufferData(GL_SHADER_STORAGE_BUFFER, data.size_bytes(), data.data(), usage);
            _size = data.size_bytes();
            unbind();
        }

        [[nodiscard]] uint32_t id() const { return _id; }
        [[nodiscard]] size_t size() const { return _size; }

    private:
        uint32_t _id{0};
        std::size_t _size{0};
    };
}

