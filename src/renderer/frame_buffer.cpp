#include "frame_buffer.hpp"
#include "glad/glad.h"

using namespace inferonix::renderer;

frame_buffer::frame_buffer(frame_buffer_settings const& settings) : _settings(settings)
{
    setup();
}

frame_buffer::~frame_buffer()
{
    release();
}

void frame_buffer::setup()
{
    if (_id)
        release();

    glGenFramebuffers(1, &_id);
    glBindFramebuffer(GL_FRAMEBUFFER, _id);

    glGenTextures(1, &_color_attachment);
    glBindTexture(GL_TEXTURE_2D, _color_attachment);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        _settings.width,
        _settings.height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        _color_attachment,
        0
    );

    glGenRenderbuffers(1, &_depth_attachment);
    glBindRenderbuffer(GL_RENDERBUFFER, _depth_attachment);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _settings.width, _settings.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depth_attachment);

    this->unbind();
}

void frame_buffer::release()
{
    if (_id)
    {
        glDeleteFramebuffers(1, &_id);
        glDeleteTextures(1, &_color_attachment);
        glDeleteRenderbuffers(1, &_depth_attachment);

        _id = 0;
        _color_attachment = 0;
        _depth_attachment = 0;
    }
}

void frame_buffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    glViewport(0, 0, _settings.width, _settings.height);
}

void frame_buffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void frame_buffer::resize(uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0 || (_settings.width == width && _settings.height == height))
        return;

    _settings.width = width;
    _settings.height = height;
    setup();
}