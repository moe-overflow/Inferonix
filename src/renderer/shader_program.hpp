#pragma once

#include "shader.hpp"
#include "color.hpp"

#include <memory>
#include <string>

#include "glm/glm.hpp"

namespace inferonix::renderer
{
    class shader_program final
    {

    public:
        constexpr static auto DEFAULT_VERTEX_SHADER =  "shaders/vertex_shader.glsl";
        constexpr static auto DEFAULT_FRAGMENT_SHADER = "shaders/fragment_shader.glsl";

        explicit shader_program(
            std::string_view vertex_shader_path = DEFAULT_VERTEX_SHADER,
            std::string_view fragment_shader_path = DEFAULT_FRAGMENT_SHADER
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

        void set_uniform(std::string_view name, color color) const;
        void set_uniform(std::string_view name, glm::mat4 mat) const;
        void set_uniform(std::string_view name, float val) const;
        void set_uniform_int(std::string_view name, int val) const;

        [[nodiscard]] uint32_t get() const
        {
            return _id;
        }


    private:
        std::unique_ptr<shader> _vertex_shader;
        std::unique_ptr<shader> _fragment_shader;
        uint32_t _id{};
    };

}
