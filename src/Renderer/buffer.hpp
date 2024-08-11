#pragma once

#include <memory>
#include <vector>
#include <glad/glad.h>

namespace Inferonix::Renderer
{

    class buffer
    {
    public:

        virtual ~buffer() { if (_id) glDeleteBuffers(1, _id.get() ); }
        virtual void bind() const = 0;
        virtual void unbind() const = 0;


        [[nodiscard]] inline uint32_t &get() const { return *_id; }

    protected:
        //buffer() = default;
        buffer() : _id(std::make_unique<uint32_t>(0)) { glGenBuffers(1, _id.get() ); }
        std::unique_ptr<uint32_t> _id;

    };

    class vertex_buffer : buffer
    {
    public:
        vertex_buffer() = delete;
        //vertex_buffer();
        explicit vertex_buffer(size_t size, const void *vertex_data);
        ~vertex_buffer() override = default;
        vertex_buffer(const vertex_buffer& other);
        vertex_buffer& operator=(const vertex_buffer& other);

        void bind() const override;
        void unbind() const override;

        void buffer_data(size_t size, const void * positions) const;

        [[nodiscard]] size_t size() const { return _size; }

    private:
        uint32_t _size{};

    };

    class index_buffer : buffer
    {
    public:
        index_buffer() = delete;
        index_buffer(size_t count, const void* indices);
        ~index_buffer() override = default;
        index_buffer(const index_buffer& other);

        index_buffer& operator=(const index_buffer& other);

        void bind() const override;
        void unbind() const override;

        void buffer_data(size_t count, const void * indices);

        [[nodiscard]] size_t count() const { return _count; }

    private:
        int64_t _count{};
    };

    enum shader_data_type { NONE = 0, FLOAT, UINT, UBYTE };

    struct vertex_buffer_element
    {
        shader_data_type type;
        uint32_t size;
        bool normalized;

        static uint32_t get_type_size(shader_data_type type)
        {
            switch (type)
            {
                case NONE: return 0;
                case FLOAT: return 4;
                case UINT: return 4;
                case UBYTE: return 1;
                default: return 0;
            }
        }

        static GLenum to_gl_type(shader_data_type type)
        {
            switch (type)
            {
                case NONE: return 0;
                case FLOAT: return GL_FLOAT;
                case UINT: return GL_UNSIGNED_INT;
                case UBYTE: return GL_UNSIGNED_BYTE;
                default: return 0;
            }
        }

    };

    class vertex_buffer_layout
    {
    public:
        vertex_buffer_layout() : _stride(0) {}

        [[nodiscard]] inline std::vector<vertex_buffer_element> get_elements() const { return _elements; }
        [[nodiscard]] inline unsigned int get_stride() const { return _stride; }

        void push(shader_data_type type, uint32_t count)
        {
            _elements.push_back({type,count,false});
            _stride += count * vertex_buffer_element::get_type_size(type);
        }

    private:
        std::vector<vertex_buffer_element> _elements;
        uint32_t _stride {0};

    };

}