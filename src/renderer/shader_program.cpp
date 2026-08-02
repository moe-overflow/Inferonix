#include "shader_program.hpp"

#include "inferonix_pch.hpp"


using namespace inferonix::renderer;

shader_program::shader_program(std::string_view vertex_shader_path, std::string_view fragment_shader_path)
    : _vertex_shader(VERTEX, vertex_shader_path.data()),
      _fragment_shader(FRAGMENT, fragment_shader_path.data()),
      _id(glCreateProgram())
{
    attach_shaders();
    if (const auto link_result = link(); !link_result)
        throw std::runtime_error("Failed to link shader program");

    glDetachShader(_id, _vertex_shader.get());
    glDetachShader(_id, _fragment_shader.get());
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
    if (_id)
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
    glAttachShader(_id, _vertex_shader.get());
    glAttachShader(_id, _fragment_shader.get());
}

std::expected<void, std::string> shader_program::link() const
{
    glLinkProgram(_id);

    auto success = GLint{};
    glGetProgramiv(_id, GL_LINK_STATUS, &success);

    if (success)
        return {};

    auto length = GLint{};
    glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);

    auto log = std::string(length, '\0');
    glGetProgramInfoLog(_id, length, nullptr, log.data());

    return std::unexpected(std::move(log));
}

GLint shader_program::get_uniform_location(std::string_view name)
{
    if (auto const it = _uniform_cache.find(name.data()); it != _uniform_cache.end())
        return it->second;

    auto location = GLint { glGetUniformLocation(_id, name.data()) };
    _uniform_cache.emplace(name, location);
    return location;
}



