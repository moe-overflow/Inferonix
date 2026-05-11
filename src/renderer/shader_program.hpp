#pragma once

#include "shader.hpp"
#include "color.hpp"

#include <memory>
#include <string>

#include "glm/glm.hpp"

namespace inferonix::renderer
{
    class shader_program
    {

    public:
        explicit shader_program(
            std::string const& vertex_shader_path = SHADERS_PATH "/vertex_shader.glsl",
            std::string const& fragment_shader_path = SHADERS_PATH "/fragment_shader.glsl"
        );

        ~shader_program();

        shader_program(shader_program const&) = delete;
        shader_program& operator=(shader_program const&) = delete;

        shader_program(shader_program&& other) noexcept;
        shader_program& operator=(shader_program&& other) noexcept;

        void use() const;
        void unuse() const;

        void attach_shaders() const;
        void link() const;

        void check_errors() const;

        void set_uniform(std::string const& name, color color) const;
        void set_uniform(std::string const& name, glm::mat4 mat) const;
        void set_uniform(std::string const& name, float val) const;
        void set_uniform_int(std::string const& name, int val) const;

        [[nodiscard]] uint32_t get() const
        {
            return *_id;
        }


    private:
        std::unique_ptr<shader> _vertex_shader;
        std::unique_ptr<shader> _fragment_shader;
        std::unique_ptr<uint32_t> _id;
    };

}
