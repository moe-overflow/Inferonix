#include "Shader.hpp"

#include "glad/glad.h"
#include "spdlog/spdlog.h"
#include <filesystem>
#include <fstream>

using namespace Inferonix::Renderer;

Shader::Shader(ShaderType const type, char const* path)
    : _type{ type },
      _id{ std::make_unique<uint32_t>(glCreateShader(type)) },
      _src_stream{ std::make_unique<std::string>(ReadFromFile(path)) }
{
    Create();
}

Shader::~Shader()
{
    glDeleteShader(*_id);
}

Shader::Shader(Shader&& other) noexcept
    : _id(std::move(other._id)),
      _src_stream(std::move(other._src_stream)),
      _type(other._type)
{
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        using std::swap;
        swap(_id, other._id);
        swap(_src_stream, other._src_stream);
        swap(_type, other._type);
    }
    return *this;
}


void Shader::Create() const
{
    auto const src = _src_stream->c_str();
    glShaderSource(*_id, 1, &src, nullptr);
    glCompileShader(*_id);

    CheckErrors();
}

GLuint Shader::Get() const
{
    return *_id;
}

void Shader::CheckErrors() const
{
    // Checking run time errors after calling 'glCompileShader()'
    int result;
    char message[512];
    glGetShaderiv(*_id, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(*_id, 512, nullptr, message);
        throw std::runtime_error("An error occurred when compiling shaders: {}" + std::string(message));
    }
}

std::string Shader::ReadFromFile(std::string const& path)
{
    try
    {
        std::string shader_code;
        std::ifstream source;
        spdlog::info("Reading shaders source from file {}", path);

        if (!std::filesystem::exists(path))
            spdlog::error("Shader file could not be found!");
        source.open(path);
        std::stringstream source_stream;
        source_stream << source.rdbuf();
        source.close();
        shader_code = source_stream.str();

        return shader_code;
    }
    catch (std::ifstream::failure const& e)
    {
        spdlog::error("Error occurred while reading shaders from disk: {}", e.what());
    }
    return "";
}
