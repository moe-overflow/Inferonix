#pragma once

#include "glad/glad.h"
#include <vector>

#include "mesh.hpp"

namespace Inferonix::Renderer
{

    class buffer
    {
    public:
        virtual ~buffer()
        {
            if (_id)
                glDeleteBuffers(1, &_id);
        }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;


        [[nodiscard]] uint32_t& Get()
        {
            return _id;
        }

    protected:
        buffer() : _id(0)
        {
            glGenBuffers(1, &_id);
        }
        uint32_t _id;
    };

    class vertex_buffer final : public buffer
    {
    public:
        vertex_buffer() = default;
        explicit vertex_buffer(size_t size);

        ~vertex_buffer() override = default;

        void Bind() const override;
        void Unbind() const override;

        void BufferData(std::vector<Vertex> const& vertices);

        [[nodiscard]] size_t Size() const
        {
            return _size;
        }

    private:
        uint32_t _size{0};
    };

    class index_buffer final : public buffer
    {
    public:
        index_buffer() = default;
        ~index_buffer() override = default;

        void Bind() const override;
        void Unbind() const override;

        void BufferData(std::vector<uint32_t>& indices);

        [[nodiscard]] int32_t Count() const
        {
            return _count;
        }

    private:
        uint32_t _count{};
    };

    enum ShaderDatatype
    {
        NONE = 0,
        FLOAT,
        UINT,
        UBYTE
    };

    struct VertexBufferElement
    {
        ShaderDatatype type;
        uint32_t size;
        bool normalized;

        static uint32_t GetTypeSize(ShaderDatatype type)
        {
            switch (type)
            {
                case NONE:
                    return 0;
                case FLOAT:
                case UINT:
                    return 4;
                case UBYTE:
                    return 1;
                default:
                    return 0;
            }
        }

        static GLenum ToGlType(ShaderDatatype type)
        {
            switch (type)
            {
                case NONE:
                    return 0;
                case FLOAT:
                    return GL_FLOAT;
                case UINT:
                    return GL_UNSIGNED_INT;
                case UBYTE:
                    return GL_UNSIGNED_BYTE;
                default:
                    return 0;
            }
        }
    };

    class VertexBufferLayout
    {
    public:
        VertexBufferLayout() = default;

        [[nodiscard]] std::vector<VertexBufferElement> GetElements() const
        {
            return _elements;
        }
        [[nodiscard]] unsigned int GetStride() const
        {
            return _stride;
        }

        void Push(ShaderDatatype const type, uint32_t const count)
        {
            _elements.push_back({ type, count, false });
            _stride += count * VertexBufferElement::GetTypeSize(type);
        }

    private:
        std::vector<VertexBufferElement> _elements;
        uint32_t _stride{ 0 };
    };

}
