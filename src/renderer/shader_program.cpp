#include "shader_program.hpp"

#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

using namespace inferonix::renderer;

namespace
{
    auto try_retrieve_uniform_location(std::string_view uniform_name, uint32_t id) -> uint32_t
    {
        auto const location = glGetUniformLocation(id, std::string(uniform_name).c_str());
        assert(location != -1);
        return location;
    }
}

shader_program::shader_program(std::string_view vertex_shader_path, std::string_view fragment_shader_path)
    : _vertex_shader(std::make_unique<shader>(VERTEX, vertex_shader_path.data())),
      _fragment_shader(std::make_unique<shader>(FRAGMENT, fragment_shader_path.data())),
      _id(glCreateProgram())
{
    attach_shaders();
    link();
    check_errors();

    glDetachShader(_id, _vertex_shader->get());
    // glDeleteShader(_vertex_shader->get());

    glDetachShader(_id, _fragment_shader->get());
    // glDeleteShader(_fragment_shader->get());
}

shader_program::shader_program(shader_program&& other) noexcept
    : _vertex_shader(std::move(other._vertex_shader)),
      _fragment_shader(std::move(other._fragment_shader)),
      _id(other._id)
{
    other._id = 0;
}

shader_program& shader_program::operator=(shader_program&& other) noexcept
{
    if (this != &other)
    {
        if (_id != 0)
            glDeleteProgram(_id);

        _vertex_shader = std::move(other._vertex_shader);
        _fragment_shader = std::move(other._fragment_shader);
        _id = other._id;
        other._id = 0;
    }
    return *this;
}

shader_program::~shader_program()
{
    glUseProgram(0);
    glDeleteProgram(_id);
}


void shader_program::use() const
{
    glUseProgram(_id);
}

void shader_program::unuse() const
{
    glUseProgram(0);
}

void shader_program::attach_shaders() const
{
    glAttachShader(_id, _vertex_shader->get());
    glAttachShader(_id, _fragment_shader->get());
}

void shader_program::link() const
{
    glLinkProgram(_id);
}

void shader_program::check_errors() const
{
    // Checking compile time errors after calling 'glCompileShader()'
    int result;
    char message[512];
    glGetProgramiv(_id, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(_id, 512, nullptr, message);
        spdlog::error("An error occurred when compiling shaders: {}", message);
    }

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        spdlog::error("OpenGL error: {}", error);
    }
}

void shader_program::set_uniform(std::string_view name, color const color) const
{
    glUniform4f(try_retrieve_uniform_location(name, this->get()), color.r, color.g, color.b, 1.0f);
}

void shader_program::set_uniform(std::string_view name, glm::mat4 mat) const
{
    glUniformMatrix4fv(try_retrieve_uniform_location(name, this->get()), 1, GL_FALSE, &mat[0][0]);
}


void shader_program::set_uniform_int(std::string_view name, int val) const
{
    glUniform1i(try_retrieve_uniform_location(name, this->get()), val);
}


void shader_program::set_uniform(std::string_view name, float val) const
{
    glUniform1f(try_retrieve_uniform_location(name, this->get()), val);
}


