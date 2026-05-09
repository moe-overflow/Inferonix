#include "layer.hpp"

#include <imgui.h>
#include "renderer/frame_buffer.hpp"

using namespace inferonix::ui;
using namespace ImGui;

scene_layer::scene_layer(std::shared_ptr<renderer::frame_buffer> framebuffer) :
   _framebuffer(std::move(framebuffer))
{}


void scene_layer::on_render()
{
    Begin("Scene Layer");

    auto const viewport_panel_size = GetContentRegionAvail();
    if (viewport_panel_size.x > 0 &&
        viewport_panel_size.y > 0 &&
        (_framebuffer->get_settings().width != viewport_panel_size.x ||
            _framebuffer->get_settings().height != viewport_panel_size.y)
    )
    {
        _framebuffer->resize(
         static_cast<uint32_t>(viewport_panel_size.x),
         static_cast<uint32_t>(viewport_panel_size.y
        ));
    }

    uint32_t const textureID = _framebuffer->get_color_attachment_renderer();
    Image(
        reinterpret_cast<ImTextureID>(reinterpret_cast<void*>(textureID)),
        ImVec2{
            static_cast<float>(_framebuffer->get_settings().width),
            static_cast<float>(_framebuffer->get_settings().height)
        },
        ImVec2{ 0, 1 },
        ImVec2{ 1, 0 }
    );

    End();

}


