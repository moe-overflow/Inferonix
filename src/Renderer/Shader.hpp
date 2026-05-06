#pragma once

#include "glad/glad.h"
#include <memory>
#include <string>

namespace Inferonix::Renderer
{

    enum ShaderType : GLenum
    {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,

    };


    class Shader
    {

    public:
        Shader() = delete;
        explicit Shader(ShaderType type, char const* src);

        ~Shader();

        Shader(Shader const&) = delete;
        Shader(Shader&& other) noexcept;

        Shader& operator=(Shader const&) = delete;
        Shader& operator=(Shader&& other) noexcept;

        static std::string ReadFromFile(std::string const& path);

        void Create() const;

        [[nodiscard]] GLuint Get() const;

        void CheckErrors() const;

    private:
        ShaderType _type;
        std::unique_ptr<uint32_t> _id;
        std::unique_ptr<std::string> _src_stream;
    };
}
