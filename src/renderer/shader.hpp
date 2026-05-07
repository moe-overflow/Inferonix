#pragma once

#include "glad/glad.h"
#include <memory>
#include <string>

namespace inferonix::renderer
{

    enum ShaderType : GLenum
    {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,

    };


    class shader
    {

    public:
        shader() = delete;
        explicit shader(ShaderType type, char const* src);

        ~shader();

        shader(shader const&) = delete;
        shader(shader&& other) noexcept;

        shader& operator=(shader const&) = delete;
        shader& operator=(shader&& other) noexcept;

        static std::string read_from_file(std::string const& path);

        void create() const;

        [[nodiscard]] GLuint get() const;

        void check_errors() const;

    private:
        ShaderType _type;
        std::unique_ptr<uint32_t> _id;
        std::unique_ptr<std::string> _src_stream;
    };
}
