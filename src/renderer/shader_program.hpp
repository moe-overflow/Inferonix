#pragma once

#include "shader.hpp"
#include "color.hpp"

#include <string>
#include <concepts>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace inferonix::renderer
{
    // todo: instead of int or float, use convertible to allow every thing that has to do with numbers
    template<typename T> concept uniform =
        std::same_as<T, float> ||
        std::same_as<T, int> ||
        std::same_as<T, glm::mat4> ||
        std::same_as<color, T> ||
        std::same_as<T, bool>;

    class shader_program final
    {

    public:
        constexpr static auto DEFAULT_VERTEX_SHADER_PATH =  "shaders/vertex_shader.glsl";
        constexpr static auto DEFAULT_FRAGMENT_SHADER_PATH = "shaders/fragment_shader.glsl";

        shader_program();

        explicit shader_program(
            const std::filesystem::path& vertex_shader_path, const std::filesystem::path& fragment_shader_path
        );

        explicit shader_program(const std::filesystem::path& compute_shader_path);

        ~shader_program();

        shader_program(shader_program const&) = delete;
        shader_program& operator=(shader_program const&) = delete;

        shader_program(shader_program&& other) noexcept;
        shader_program& operator=(shader_program&& other) noexcept;

        void use() const;
        void unuse() const;

        void attach_shaders() const;

        [[nodiscard]] std::expected<void, std::string> link() const;


    private:
        [[nodiscard]] GLint get_uniform_location(std::string_view name);

    public:
        template<uniform T>
        auto set_uniform(std::string_view name, const T& value) -> void
        {
            const auto location = get_uniform_location(name);
            if constexpr (std::is_same_v<T, int>)
                glUniform1i(location, value);
            else if constexpr (std::is_same_v<T, float>)
                glUniform1f(location, value);
            else if constexpr (std::is_same_v<T, glm::mat4>)
                glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
            else if constexpr (std::is_same_v<T, color>)
                glUniform4f(location, value.r, value.g, value.b, 1.0f);
            else if constexpr (std::is_same_v<T, bool>)
                glUniform1i(location, value ? 1 : 0);
        }


        [[nodiscard]] uint32_t get() const
        {
            return _id;
        }


    private:
        std::optional<shader> _vertex_shader;
        std::optional<shader> _fragment_shader;
        std::optional<shader> _compute_shader;

        uint32_t _id{};

        std::unordered_map<std::string, GLint> _uniform_cache;
    };

}
