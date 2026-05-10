#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image.h>
#include <filesystem>


namespace inferonix::renderer
{
    class texture final
    {
    public:
        texture() = default;
        ~texture() = default;

        auto load_from_file(std::filesystem::path const& path) -> bool
        {
            // flip image to match OpenGL coordinate expectation => 0.0 on bottom
            stbi_set_flip_vertically_on_load(true);
            spdlog::info("loading texture from file: {}", path.string());
            stbi_uc* data = stbi_load(path.string().c_str(), &_width, &_height, &_channels, 0);

            if (!data)
            {
                spdlog::error("Failed to load texture: {} - Reason: {}", path.string(), stbi_failure_reason());
                return false;
            }

            glGenTextures(1, &_id);
            glBindTexture(GL_TEXTURE_2D, _id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            auto format = (_channels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
            return true;
        }

        auto bind(uint32_t slot) const -> void
        {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, _id);
        }

        auto unbind() -> void const;

        [[nodiscard]] auto get_id() const -> uint32_t { return _id; }

    protected:
        uint32_t _id;
        int _width{0}, _height{0}, _channels{0};
    };
}


