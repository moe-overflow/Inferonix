#pragma once

#include <memory>
#include "renderer/frame_buffer.hpp"

namespace inferonix::ui
{


    class layer
    {
    public:
        virtual ~layer() = default;

        virtual void on_render(){}
        virtual void on_update(){}
        virtual void on_attach(){}

    };

    class dockspace final : public layer
    {
    public:
        void on_render() override;

    };

    class scene_layer final : public layer
    {
    public:
        explicit scene_layer(std::shared_ptr<renderer::frame_buffer> framebuffer);
        void on_render() override;

    private:
        std::shared_ptr<renderer::frame_buffer> _framebuffer;

    };


}

