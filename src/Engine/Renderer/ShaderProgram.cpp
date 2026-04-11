#include "ShaderProgram.hpp"

#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include <cmath>

using namespace Inferonix::Renderer;

ShaderProgram::ShaderProgram(
            std::string const& VertexShaderPath,
            std::string const& FragmentShaderPath
)
    : _vertex_shader(std::make_unique<Shader>(VERTEX, VertexShaderPath.c_str())),
      _fragment_shader(std::make_unique<Shader>(FRAGMENT, FragmentShaderPath.c_str())),
      _id(std::make_unique<uint32_t>(glCreateProgram()))
{
    AttachShaders();
    Link();
    CheckErrors();

    glDetachShader(*_id, _vertex_shader->Get());
    glDeleteShader(_vertex_shader->Get());

    glDetachShader(*_id, _fragment_shader->Get());
    glDeleteShader(_fragment_shader->Get());
}

ShaderProgram::~ShaderProgram()
{
    glUseProgram(0);
    glDeleteProgram(*_id);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    : _id(std::move(other._id)),
      _vertex_shader(std::move(other._vertex_shader)),
      _fragment_shader(std::move(other._fragment_shader))
{
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    if (this != &other)
    {
        using std::swap;
        swap(_id, other._id);
        swap(_vertex_shader, other._vertex_shader);
        swap(_fragment_shader, other._fragment_shader);
    }
    return *this;
}

void ShaderProgram::Use() const
{
    glUseProgram(*_id);
}

void ShaderProgram::Unuse() const
{
    glUseProgram(0);
}

void ShaderProgram::AttachShaders() const
{
    glAttachShader(*_id, _vertex_shader->Get());
    glAttachShader(*_id, _fragment_shader->Get());
}

void ShaderProgram::Link() const
{
    glLinkProgram(*_id);
}

void ShaderProgram::CheckErrors() const
{
    // Checking compile time errors after calling 'glCompileShader()'
    int result;
    char message[512];
    glGetProgramiv(*_id, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(*_id, 512, nullptr, message);
        spdlog::error("An error occurred when compiling shaders: {}", message);
    }

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        spdlog::error("OpenGL error: {}", error);
    }
}

//todo: use vector instead of three rgb values
void ShaderProgram::SetUniform(std::string const& name, float r, float g, float b) const
{
    int location = glGetUniformLocation(this->Get(), name.c_str());
    assert(location != -1);
    glUniform4f(location, r, g, b, 1.0f);
}

void ShaderProgram::SetUniform(std::string const& name, glm::mat4 mat) const
{
    int location = glGetUniformLocation(this->Get(), name.c_str());
    assert(location != -1);
    glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::SetDynamicColor(std::string const& uniform_name) const
{
    auto time_value = static_cast<float>(glfwGetTime());

    auto green = static_cast<float>(cos(static_cast<double>(time_value)) * 0.5 + 0.5);
    auto blue = static_cast<float>(sin(static_cast<double>(time_value)) * 0.5 + 0.5);
    auto red = 1.0f;

    SetUniform(uniform_name, red, green, blue);
}
