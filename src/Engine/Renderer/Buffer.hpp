#pragma once

#include "glad/glad.h"
#include <vector>

#include "Mesh.hpp"

namespace Inferonix::Renderer
{

    class Buffer
    {
    public:
        virtual ~Buffer()
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
        Buffer() : _id(0)
        {
            glGenBuffers(1, &_id);
        }
        uint32_t _id;
    };

    class VertexBuffer final : public Buffer
    {
    public:
        VertexBuffer() = default;
        explicit VertexBuffer(size_t size);

        ~VertexBuffer() override = default;

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

    class IndexBuffer final : public Buffer
    {
    public:
        IndexBuffer() = default;
        ~IndexBuffer() override = default;

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
