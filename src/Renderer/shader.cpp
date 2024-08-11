
#include "shader.hpp"

#include <glad/glad.h>
#include <fstream>
#include <filesystem>
#include <spdlog/spdlog.h>

using namespace Inferonix::Renderer;

shader::shader(shader_type type, const char* path) :
        _type { type },
        _id { std::make_unique<uint32_t>(glCreateShader(type)) },
        _src_stream { std::make_unique<std::string>(read_from_file(path)) }
{

    _src = _src_stream->c_str();
    create();
}


shader::~shader(){
    glDeleteShader(*_id);
}

void shader::create(){
    glShaderSource(*_id, 1, &_src, nullptr);
    glCompileShader(*_id);

    check_errors();

}

GLuint shader::get() const {
    return *_id;
}

void shader::check_errors() const {
    // Checking run time errors after calling 'glCompileShader()'
    int result;
    char message[512];
    glGetShaderiv(*_id, GL_COMPILE_STATUS, &result);
    if(!result) {
        glGetShaderInfoLog(*_id, 512, nullptr, message);
        throw std::runtime_error("An error occurred when compiling shaders: {}" + std::string(message));
    }
}

std::string shader::read_from_file(const std::string& path) {
    std::ifstream source;
    std::string shader_code;
    try {
        spdlog::info("Reading shaders source from file {}", path);
        bool found = std::filesystem::exists(path);
        if (!found)
            spdlog::error("Shader file could not be found!");
        source.open(path);
        std::stringstream source_stream;
        source_stream << source.rdbuf();
        source.close();
        shader_code = source_stream.str();

        return shader_code;

    } catch (const std::ifstream::failure& e) {
        spdlog::error("Error occurred while reading shaders from disk: {}", e.what());
    }
    return "";
}


