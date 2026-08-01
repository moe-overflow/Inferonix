#pragma once

#include <memory>

#include "renderer/frame_buffer.hpp"
#include "scene/scene.hpp"


namespace inferonix::ui
{
    class scene_hierarchy;

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
        explicit scene_layer(
            std::shared_ptr<renderer::frame_buffer> framebuffer,
            std::shared_ptr<scene::scene> scene,
            scene_hierarchy& hierarchy_layer
        );

    void on_render() override;

    private:
        void render_gizmos(const std::pair<int, int>& viewport_min, const std::pair<int, int>& viewport_size) const;

    private:
        std::shared_ptr<renderer::frame_buffer> _framebuffer;
        std::shared_ptr<scene::scene> _scene;
        scene_hierarchy* _hierarchy;
    };


}

