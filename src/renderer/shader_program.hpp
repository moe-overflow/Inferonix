#pragma once

#include "shader.hpp"
#include <memory>
#include <string>

#include "glm/glm.hpp"

namespace Inferonix::Renderer
{

    class shader_program
    {

    public:
        explicit shader_program(
            std::string const& VertexShaderPath = SHADERS_PATH "/vertex_shader.glsl",
            std::string const& FragmentShaderPath = SHADERS_PATH "/fragment_shader.glsl"
        );

        ~shader_program();

        shader_program(shader_program const&) = delete;
        shader_program& operator=(shader_program const&) = delete;

        shader_program(shader_program&& other) noexcept;
        shader_program& operator=(shader_program&& other) noexcept;

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
        std::unique_ptr<shader> _vertex_shader;
        std::unique_ptr<shader> _fragment_shader;
        std::unique_ptr<uint32_t> _id;
    };

}
