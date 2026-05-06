#pragma once

#include "Shader.hpp"
#include <memory>
#include <string>

#include "glm/glm.hpp"

namespace Inferonix::Renderer
{

    class ShaderProgram
    {

    public:
        explicit ShaderProgram(
            std::string const& VertexShaderPath = SHADERS_PATH "/VertexShader.glsl",
            std::string const& FragmentShaderPath = SHADERS_PATH "/FragmentShader.glsl"
        );

        ~ShaderProgram();

        ShaderProgram(ShaderProgram const&) = delete;
        ShaderProgram& operator=(ShaderProgram const&) = delete;

        ShaderProgram(ShaderProgram&& other) noexcept;
        ShaderProgram& operator=(ShaderProgram&& other) noexcept;

        void Use() const;
        void Unuse() const;

        void AttachShaders() const;
        void Link() const;

        void CheckErrors() const;

        void SetUniform(std::string const& name, float r, float g, float b) const;

        void SetUniform(std::string const& name, glm::mat4 mat) const;

        [[nodiscard]] uint32_t Get() const
        {
            return *_id;
        }

        void SetDynamicColor(std::string const& uniform_name) const;

    private:
        std::unique_ptr<Shader> _vertex_shader;
        std::unique_ptr<Shader> _fragment_shader;
        std::unique_ptr<uint32_t> _id;
    };

}
