#include "shader_program.hpp"

#include "GLFW/glfw3.h"
#include <cmath>
#include <spdlog/spdlog.h>

using namespace Inferonix::Renderer;

shader_program::shader_program() : // todo: pass path as param
    _vertex_shader(std::make_unique<shader>(shader_type::VERTEX, "shaders/vertex_shader.glsl")),
    _fragment_shader(std::make_unique<shader>(shader_type::FRAGMENT, "shaders/fragment_shader.glsl")),
    _id (std::make_unique<uint32_t>(glCreateProgram()))
{

    attach_shaders();
    link();
    check_errors();

    /* todo: move to separate function or two */

    glDetachShader(*_id, _vertex_shader->get());
    glDeleteShader(_vertex_shader->get());

    glDetachShader(*_id, _fragment_shader->get());
    glDeleteShader(_fragment_shader->get());


}

shader_program::~shader_program()
{
    glUseProgram(0);
    glDeleteProgram(*_id);
}

void shader_program::use() const
{
    glUseProgram(*_id);
}

void shader_program::unuse() const
{
    glUseProgram(0);
}

void shader_program::attach_shaders()
{
    glAttachShader(*_id, _vertex_shader->get());
    glAttachShader(*_id, _fragment_shader->get());
}

void shader_program::link()
{
    glLinkProgram(*_id);
}

void shader_program::check_errors() const
{
    // Checking compile time errors after calling 'glCompileShader()'
    int result;
    char message[512];
    glGetProgramiv(*_id, GL_LINK_STATUS, &result);
    if(!result) {
        glGetProgramInfoLog(*_id, 512, nullptr, message);
        spdlog::error("An error occurred when compiling shaders: {}", message);
    }

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        spdlog::error("OpenGL error: {}", error);
    }

}

//todo: use vector instead of three rgb values
void shader_program::set_uniform(const std::string& name, float r, float g, float b) const
{
    int location = glGetUniformLocation(this->get(), name.c_str());
    assert(location != -1);
    glUniform4f(location, r, g, b, 1.0f);
}

void shader_program::set_dynamic_color(const std::string& uniform_name) const
{
    auto time_value = static_cast<float>(glfwGetTime());

    auto green = static_cast<float>(cos(static_cast<double>(time_value)) * 0.5 + 0.5);
    auto blue = static_cast<float>(sin(static_cast<double>(time_value)) * 0.5 + 0.5);
    auto red = blue * green;

    set_uniform(uniform_name, red, green, blue);

}




