#include "layer.hpp"

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

#include <renderer/frame_buffer.hpp>
#include <scene/components.hpp>
#include <ui/scene_hierarchy.hpp>

using namespace inferonix::ui;
using namespace ImGui;

scene_layer::scene_layer(
        std::shared_ptr<renderer::frame_buffer> framebuffer,
        std::shared_ptr<scene::scene> scene,
        scene_hierarchy& hierarchy_layer
    ) :
        _framebuffer(std::move(framebuffer)),
        _scene(std::move(scene)),
        _hierarchy(&hierarchy_layer)
{}


void scene_layer::render_gizmos() const
{
    const auto selected_entity = entt::entity{ _hierarchy->get_selected_entity() };
    auto& registry = _scene->get_registry();

    // ensure entity is registered already
    if (selected_entity == entt::null || !registry.valid(selected_entity))
        return;

    // ensure entity contains transform data
    if (!registry.any_of<scene::transform_component>(selected_entity))
        return;

    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(
        GetWindowPos().x, GetWindowPos().y,
        GetWindowSize().x, GetWindowSize().y
    );

    const auto camera = _scene->get_editor_camera();
    auto view_matrix = camera->get_view();
    auto projection_matrix = camera->get_projection();

    auto& transform_component = registry.get<scene::transform_component>(selected_entity);
    auto transform_matrix = transform_component.get_matrix();

    ImGuizmo::Manipulate(
        glm::value_ptr(view_matrix),
        glm::value_ptr(projection_matrix),
        ImGuizmo::TRANSLATE,
        ImGuizmo::LOCAL,
        glm::value_ptr(transform_matrix),
        nullptr,
        nullptr
    );

    if (ImGuizmo::IsUsing())
        transform_component.set_from_matrix(transform_matrix);

    camera->set_block_input(ImGuizmo::IsUsing() || ImGuizmo::IsOver());

}


void scene_layer::on_render()
{
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ .0f, .0f});
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
            static_cast<uint32_t>(viewport_panel_size.y)
        );
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

    render_gizmos();

    End();
    PopStyleVar();
}


