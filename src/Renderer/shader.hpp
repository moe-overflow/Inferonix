#pragma once

#include <glad/glad.h>
#include <string>
#include <string_view>
#include <memory>

namespace Engine::Renderer
{

    enum shader_type : GLenum
    {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,

    };


    class shader
    {

    public:
        shader() = delete;
        explicit shader(shader_type type, const char* src);
        shader(shader const&) = delete;
        shader(shader&&) = delete;
        ~shader();
        static std::string read_from_file(const std::string& path);
        void create();
        [[nodiscard]] GLuint get() const;
        void check_errors() const;

    private:

        shader_type _type;
        std::unique_ptr<uint32_t> _id;
        std::unique_ptr<std::string> _src_stream;
        const char* _src;

    };
}
