#pragma once

#include <assimp/texture.h>
#include <glad/glad.h>
#include <stb_image.h>

#include <filesystem>

#include "util/logger.hpp"

namespace inferonix::asset{ class asset_registry; }
namespace inferonix::renderer
{
class texture final
{
public:
    texture() = default;

    ~texture()
    {
        if (_id)
            glDeleteTextures(1, &_id);
    }

    texture(const texture&) = delete;
    texture& operator=(const texture&) = delete;

    texture(texture&& other) noexcept
    {
        _id = std::exchange(other._id, 0);
    }

    texture& operator=(texture&& other) noexcept
    {
        if (this != &other)
        {
            if (_id)
                glDeleteTextures(1, &_id);

            _id = std::exchange(other._id, 0);
        }

        return *this;
    }

    bool load(const std::filesystem::path& path, [[maybe_unused]] asset::asset_registry& registry)
    {
        stbi_set_flip_vertically_on_load(true);

        int width;
        int height;
        int channels;

        stbi_uc* pixels = stbi_load(
            path.string().c_str(),
            &width,
            &height,
            &channels,
            0);

        if (!pixels)
        {
            LOG(LOG_TYPE::ERROR,
                "Failed to load texture '{}': {}",
                path.string(),
                stbi_failure_reason());

            return false;
        }

        create_gl_texture(
            pixels,
            width,
            height,
            channels);

        stbi_image_free(pixels);

        return true;
    }

    bool load_from_assimp(const aiTexture* embedded)
    {
        if (!embedded)
            return false;

        int width;
        int height;
        int channels;

        unsigned char* pixels = nullptr;

        if (embedded->mHeight == 0)
        {
            pixels = stbi_load_from_memory(
                reinterpret_cast<unsigned char*>(embedded->pcData),
                embedded->mWidth,
                &width,
                &height,
                &channels,
                0);
        }
        else
        {
            width = embedded->mWidth;
            height = embedded->mHeight;
            channels = 4;

            pixels = new unsigned char[width * height * 4];

            memcpy(
                pixels,
                embedded->pcData,
                width * height * 4);
        }

        if (!pixels)
        {
            LOG(LOG_TYPE::ERROR,
                "Failed to decode embedded texture.");

            return false;
        }

        create_gl_texture(
            pixels,
            width,
            height,
            channels);

        if (embedded->mHeight == 0)
            stbi_image_free(pixels);
        else
            delete[] pixels;

        return true;
    }

    void bind(GLuint slot = 0) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, _id);
    }

    static void unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    [[nodiscard]]
    GLuint id() const
    {
        return _id;
    }

private:
    void create_gl_texture(
        const unsigned char* pixels,
        int width,
        int height,
        int channels)
    {
        GLenum format = channels == 4 ? GL_RGBA : GL_RGB;

        glGenTextures(1, &_id);

        glBindTexture(
            GL_TEXTURE_2D,
            _id);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_S,
            GL_REPEAT);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_T,
            GL_REPEAT);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            width,
            height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            pixels);

        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    GLuint _id = 0;
};
}