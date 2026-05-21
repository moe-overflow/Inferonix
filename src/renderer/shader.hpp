#pragma once

#include "glad/glad.h"
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
        explicit shader(ShaderType type, std::string_view src);

        ~shader();

        shader(shader const&) = delete;
        shader(shader&& other) noexcept = default;

        shader& operator=(shader const&) = delete;
        shader& operator=(shader&& other) noexcept = default;

        static std::string read_from_file(std::string_view path);

        void create() const;

        [[nodiscard]] uint32_t get() const;

        void check_errors() const;

    private:
        ShaderType _type;
        uint32_t _id;
        std::string _src_stream;
    };
}
