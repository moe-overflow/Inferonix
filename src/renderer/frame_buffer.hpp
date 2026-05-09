#pragma once

#include <cstdint>

namespace inferonix::renderer
{
    class frame_buffer final
    {
    public:

        struct frame_buffer_settings
        {
            int width, height;
        };

        explicit frame_buffer(frame_buffer_settings const& settings);

        ~frame_buffer();

        void setup();

        void bind() const;
        void unbind() const;

        [[nodiscard]] frame_buffer_settings& get_settings()
        {
            return _settings;
        }

        [[nodiscard]] uint32_t get_color_attachment_renderer() const { return _color_attachment; }


        void resize(uint32_t width, uint32_t height);

    private:
        void release();

        uint32_t _id = 0;
        uint32_t _color_attachment = 0, _depth_attachment = 0;
        frame_buffer_settings _settings;
    };


}