#pragma once

#include <cstdint>

namespace inferonix::renderer
{
    class frame_buffer final
    {
    public:

        struct frame_buffer_settings
        {
            int width { 0 }, height { 0 };
        };

        explicit frame_buffer(frame_buffer_settings const& settings);

        ~frame_buffer();

        void setup();

        void bind() const;
        void unbind() const;

        [[nodiscard]] frame_buffer_settings get_settings() const
        {
            return _settings;
        }

        // to draw visual scene
        [[nodiscard]] uint32_t get_color_attachment_renderer() const { return _color_attachment; }

        // to store raw integer entity IDs
        [[nodiscard]] uint32_t get_entity_id_attachment() const { return _entity_id_attachment; }

        // reads a pixel value from the entity ID attachment at (x, y)
        [[nodiscard]] int read_pixel(uint32_t attachment_index, int x, int y) const;

        void resize(uint32_t width, uint32_t height);

        void clear_attachment(uint32_t attachment_index, int value) const;

    private:
        void release();

        uint32_t _id = 0;
        uint32_t _color_attachment = 0;
        uint32_t _depth_attachment = 0;
        uint32_t _entity_id_attachment = 0;

        frame_buffer_settings _settings;
    };


}