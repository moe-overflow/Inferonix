#include "shader.hpp"

#include "glad/glad.h"
#include "spdlog/spdlog.h"
#include <filesystem>
#include <fstream>

using namespace Inferonix::Renderer;

shader::shader(ShaderType const type, char const* src)
    : _type{ type },
      _id{ std::make_unique<uint32_t>(glCreateShader(type)) },
      _src_stream{ std::make_unique<std::string>(ReadFromFile(src)) }
{
    Create();
}

shader::~shader()
{
    glDeleteShader(*_id);
}

shader::shader(shader&& other) noexcept
    : _type(other._type),
      _id(std::move(other._id)),
      _src_stream(std::move(other._src_stream))
{
}

shader& shader::operator=(shader&& other) noexcept
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


void shader::Create() const
{
    auto const src = _src_stream->c_str();
    glShaderSource(*_id, 1, &src, nullptr);
    glCompileShader(*_id);

    CheckErrors();
}

GLuint shader::Get() const
{
    return *_id;
}

void shader::CheckErrors() const
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

std::string shader::ReadFromFile(std::string const& path)
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
        return "";
    }
}
