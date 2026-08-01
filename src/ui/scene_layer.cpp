#include "layer.hpp"

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

#include <renderer/frame_buffer.hpp>
#include <scene/components.hpp>
#include <ui/scene_hierarchy.hpp>

using namespace inferonix::ui;
using namespace ImGui;

namespace
{
    auto get_entity_at_pixel(const inferonix::renderer::frame_buffer& fb, int pixel_x, int pixel_y) -> int
    {
        const auto width = fb.get_settings().width;
        const auto height = fb.get_settings().height;

        if (pixel_x >= 0 && pixel_y >= 0 && pixel_x < width && pixel_y < height)
            return fb.read_pixel(1, pixel_x, pixel_y);
        return -1;
    }

}

scene_layer::scene_layer(
        std::shared_ptr<renderer::frame_buffer> framebuffer,
        std::shared_ptr<scene::scene> scene,
        scene_hierarchy& hierarchy_layer
    ) :
        _framebuffer(std::move(framebuffer)),
        _scene(std::move(scene)),
        _hierarchy(&hierarchy_layer)
{}


void scene_layer::render_gizmos(const std::pair<int, int>& viewport_min, const std::pair<int, int>& viewport_size) const
{
    const auto selected_entity = entt::entity{ _hierarchy->get_selected_entity() };
    auto& registry = _scene->get_registry();

    // ensure entity is registered already
    if (selected_entity == entt::null || !registry.valid(selected_entity))
        return;

    // ensure entity contains transform data
    if (!registry.any_of<scene::transform_component>(selected_entity))
        return;

    static ImGuizmo::OPERATION current_operation = ImGuizmo::TRANSLATE;
    static ImGuizmo::MODE current_mode = ImGuizmo::LOCAL;

    if (!ImGuizmo::IsUsing())
    {
        if (IsKeyPressed(ImGuiKey_B)) current_operation = ImGuizmo::TRANSLATE;
        if (IsKeyPressed(ImGuiKey_N)) current_operation = ImGuizmo::ROTATE;
        if (IsKeyPressed(ImGuiKey_M)) current_operation = ImGuizmo::SCALE;
    }

    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(
        static_cast<float>(viewport_min.first),
        static_cast<float>(viewport_min.second),
        static_cast<float>(viewport_size.first),
        static_cast<float>(viewport_size.second)
    );

    const auto camera = _scene->get_editor_camera();
    auto view_matrix = camera->get_view();
    auto projection_matrix = camera->get_projection();

    auto& transform_component = registry.get<scene::transform_component>(selected_entity);
    auto transform_matrix = transform_component.get_matrix();

    float snap_value = 0.5f;
    if (current_operation == ImGuizmo::ROTATE)
        snap_value = 45.0f;

    bool const snap = GetIO().KeyCtrl;
    float const snap_values[3] = { snap_value, snap_value, snap_value };

    ImGuizmo::Manipulate(
        glm::value_ptr(view_matrix),
        glm::value_ptr(projection_matrix),
        current_operation,
        current_mode,
        glm::value_ptr(transform_matrix),
        nullptr,
        snap ? snap_values : nullptr
    );

    if (ImGuizmo::IsUsing())
        transform_component.set_from_matrix(transform_matrix);

    camera->set_block_input(ImGuizmo::IsUsing() || ImGuizmo::IsOver());

}


void scene_layer::on_render()
{
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ .0f, .0f});
    Begin("Scene Layer");

    const auto viewport_panel_size = GetContentRegionAvail();
    const auto target_width = static_cast<int>(viewport_panel_size.x);
    const auto target_height = static_cast<int>(viewport_panel_size.y);

    if (target_width > 0 && target_height > 0 &&
        (_framebuffer->get_settings().width != target_width ||
         _framebuffer->get_settings().height != target_height))
    {
        _framebuffer->resize(
            static_cast<uint32_t>(target_width),
            static_cast<uint32_t>(target_height)
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

    // screen coordinates of cursor
    const ImVec2 mouse_pos = GetMousePos();

    // top-left of viewport image
    const ImVec2 item_pos = GetItemRectMin();

    // width and height of viewport image
    const ImVec2 item_size = GetItemRectSize();

    if (IsItemHovered() && IsMouseClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsOver())
    {
        const float relative_x = mouse_pos.x - item_pos.x;
        const float relative_y = mouse_pos.y - item_pos.y;

        const int pixel_x = static_cast<int>(relative_x);
        const int pixel_y = static_cast<int>(item_size.y - relative_y);

        if (const int pixel_id = get_entity_at_pixel(*_framebuffer, pixel_x, pixel_y); pixel_id != -1)
        {
            if (auto const selected = static_cast<entt::entity>(pixel_id); _scene->get_registry().valid(selected))
                _hierarchy->set_selected_entity(selected);
        }
        else
            _hierarchy->set_selected_entity(entt::null);
    }

    render_gizmos(
        { static_cast<int>(item_pos.x), static_cast<int>(item_pos.y) },
        { static_cast<int>(item_size.x), static_cast<int>(item_size.y) }
    );

    End();
    PopStyleVar();
}


