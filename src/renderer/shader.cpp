#include "shader.hpp"

#include "inferonix_pch.hpp"
#include "glad/glad.h"
#include <filesystem>
#include <fstream>

using namespace inferonix::renderer;

shader::shader(ShaderType const type, std::string_view src)
    : _type{ type },
      _id{ glCreateShader(type) },
      _src_stream{ read_from_file(src) }
{
    create();
}

shader::~shader()
{
    glDeleteShader(_id);
}

void shader::create() const
{
    auto const src = _src_stream.c_str();
    glShaderSource(_id, 1, &src, nullptr);
    glCompileShader(_id);

    check_errors();
}

uint32_t shader::get() const
{
    return _id;
}

void shader::check_errors() const
{
    // Checking run time errors after calling 'glCompileShader()'
    int result;
    char message[512];
    glGetShaderiv(_id, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(_id, 512, nullptr, message);
        throw std::runtime_error("An error occurred when compiling shaders: {}" + std::string(message));
    }
}

std::string shader::read_from_file(std::string_view path)
{
    try
    {
        auto shader_code = std::string{};
        auto source = std::ifstream{};
        LOG(LOG_TYPE::INFO, "Reading shaders source from file {}", path);

        if (!std::filesystem::exists(path))
            spdlog::error("Shader file could not be found!");
        source.open(path.data());
        auto source_stream = std::stringstream{};
        source_stream << source.rdbuf();
        source.close();
        shader_code = source_stream.str();

        return shader_code;
    }
    catch (std::ifstream::failure const& e)
    {
        LOG(LOG_TYPE::ERROR, "Error occurred while reading shaders from disk: {}", e.what());
        return "";
    }
}
