#include "frame_buffer.hpp"

#include "scene/scene.hpp"

#include <glad/glad.h>

using namespace inferonix::renderer;


namespace
{
    auto setup_scene_attachment(uint32_t& color_attachment, const size_t width, const size_t height) -> void
    {
        glGenTextures(1, &color_attachment);
        glBindTexture(GL_TEXTURE_2D, color_attachment);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA8,
            width,
            height,
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
            color_attachment,
            0
        );
    }


    auto setup_id_attachment(uint32_t& entity_id_attachment, const size_t width, const size_t height) -> void
    {
        glGenTextures(1, &entity_id_attachment);
        glBindTexture(GL_TEXTURE_2D, entity_id_attachment);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_R32I,
            width,
            height,
            0,
            GL_RED_INTEGER,
            GL_INT,
            nullptr
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT1,
            GL_TEXTURE_2D,
            entity_id_attachment,
            0
        );
    }
}


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

    setup_scene_attachment(_color_attachment, _settings.width, _settings.height);
    setup_id_attachment(_entity_id_attachment, _settings.width, _settings.height);

    GLenum buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, buffers);


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
        glDeleteTextures(1, &_entity_id_attachment);
        glDeleteRenderbuffers(1, &_depth_attachment);

        _id = 0;
        _color_attachment = 0;
        _depth_attachment = 0;
        _entity_id_attachment = 0;
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


int frame_buffer::read_pixel(uint32_t attachment_index, int x, int y) const
{
    if (x < 0 || y < 0 || x >= _settings.width || y >= _settings.height)
        return -1;

    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);

    int pixel_data = -1;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return pixel_data;
}

void frame_buffer::clear_attachment(uint32_t attachment_index, int value) const
{
    glClearBufferiv(GL_COLOR, attachment_index, &value);
}

