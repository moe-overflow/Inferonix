#pragma once

#include "renderer/shader_program.hpp"

#include <utility>
#include <cstdint>
#include <filesystem>
#include <expected>
#include <fstream>
#include <format>

namespace inferonix::renderer
{
    class compute_shader final
    {
    public:
        compute_shader() = default;
        ~compute_shader()
        {
            if (_id != 0)
                glDeleteProgram(_id);
        }

        compute_shader(const compute_shader&) = delete;
        compute_shader& operator=(const compute_shader&) = delete;

        compute_shader(compute_shader&& other) noexcept
            : _id(std::exchange(other._id, 0)) {}

        compute_shader& operator=(compute_shader&& other) noexcept
        {
            if (this != &other)
            {
                if (_id != 0)
                    glDeleteProgram(_id);
                _id = std::exchange(other._id, 0);
            }
            return *this;
        }

        [[nodiscard]] auto load_from_file(const std::filesystem::path& filepath) -> std::expected<void, std::string>
        {
            if (!std::filesystem::exists(filepath))
                return std::unexpected(std::format("Compute shader file not found: {}", filepath.string()));

            auto file = std::ifstream{filepath};
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string source_str = buffer.str();
            const char* source = source_str.c_str();

            auto compute_shader_handle = GLuint { glCreateShader(GL_COMPUTE_SHADER)};
            glShaderSource(compute_shader_handle, 1, &source, 0);
            glCompileShader(compute_shader_handle);

            auto is_compiled = GLint{ 0 };
            glGetShaderiv(compute_shader_handle, GL_COMPILE_STATUS, &is_compiled);

            if (is_compiled == GL_FALSE)
            {
                auto max_length = GLint { 0 };
                glGetShaderiv(compute_shader_handle, GL_INFO_LOG_LENGTH, &max_length);
                auto log = std::vector<GLchar>(max_length);
                glGetShaderInfoLog(compute_shader_handle, max_length, &max_length, &log[0]);
                glDeleteShader(compute_shader_handle);
                return std::unexpected(std::format("Compute shader compilation failed: {}", std::string_view{log.data()}));
            }

            _id = GLuint { glCreateProgram() };
            glAttachShader(_id, compute_shader_handle);
            glLinkProgram(_id);

            auto is_linked = GLint{ 0 };
            glGetProgramiv(_id, GL_LINK_STATUS, &is_linked);
            if (is_linked == GL_FALSE)
            {
                auto max_length = GLint { 0 };
                glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &max_length);
                auto log = std::vector<GLchar>(max_length);
                glGetProgramInfoLog(_id, max_length, &max_length, &log[0]);
                glDeleteProgram(_id);
                _id = 0;
                return std::unexpected(std::format("Compute shader linking failed: {}", std::string_view{log.data()}));
            }

            glDeleteShader(compute_shader_handle);
            return {};
        }

        auto use() const -> void
        {
            glUseProgram(_id);
        }

        auto dispatch(const uint32_t group_x, const uint32_t group_y, const uint32_t group_z) const -> void
        {
            use();
            glDispatchCompute(group_x, group_y, group_z);
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        }

        auto set_uniform(const std::string& name, const float value) const -> void
        {
            glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
        }

        auto set_uniform(const std::string& name, const int value) const -> void
        {
            glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
        }

        [[nodiscard]] uint32_t id() const { return _id; }


    private:
        uint32_t _id { 0 };

    };
}

