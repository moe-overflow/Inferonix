#pragma once

#include "layer.hpp"
#include "scene/components.hpp"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

using namespace inferonix::ui;
using namespace ImGui;

namespace inferonix::ui
{
    class properties_layer final : public layer
    {
    public:
        properties_layer(
            std::shared_ptr<scene::scene> scene, scene_hierarchy_layer& scene_hierarchy_layer
        ) : _scene(std::move(scene)), _scene_hierarchy_layer(scene_hierarchy_layer)
        {}

        ~properties_layer() override = default;

        auto on_render() -> void override
        {
            Begin("Properties");

            auto selected = _scene_hierarchy_layer.get_selected_entity();
            if (selected != entt::null && _scene->get_registry().valid(selected))
                draw_component(selected);

            End();
        }

    private:
        auto draw_component(const scene::entity& entity) const -> void
        {
            if (auto& registry = _scene->get_registry(); registry.all_of<scene::transform_component>(entity))
            {
                if (CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    auto& transform = registry.get<scene::transform_component>(entity);

                    Text("Position");
                    SetNextItemWidth(-1.f);
                    DragFloat3("##Position", glm::value_ptr(transform.position), 0.1f);

                    Text("Rotation");
                    SetNextItemWidth(-1.f);
                    DragFloat3("##Rotation", glm::value_ptr(transform.rotation), 0.5f);

                    Text("Scale");
                    SetNextItemWidth(-1.f);
                    DragFloat3("##Scale", glm::value_ptr(transform.scale), 0.05f);
                }

                if (_scene->get_registry().any_of<scene::simulation_component>(entity))
                {
                    auto& simulation = _scene->get_registry().get<scene::simulation_component>(entity);
                    if (CollapsingHeader("Simulation", ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        {
                            Text("Particle count");
                            SetNextItemWidth(-1.f);

                            auto current_count = static_cast<uint32_t>(simulation.particles.size());
                            constexpr uint32_t min = 0;
                            constexpr uint32_t max = 30000;

                            if (DragScalar("##Particle count", ImGuiDataType_U32, &current_count, 5.f, &min, &max, "%u"))
                                simulation.particles.resize(current_count);
                        }

                        {
                            Text("Drag");
                            SetNextItemWidth(-1.f);
                            DragFloat("##Drag", &simulation.drag, .5f, -200.f, 200.f);
                        }

                        {
                            Text("Binding Point");
                            SetNextItemWidth(-1.f);
                            DragScalar("##Binding Point", ImGuiDataType_U32, &simulation.binding_point, .5f);
                        }

                    }


                }
            }
        }

    private:
        std::shared_ptr<scene::scene> _scene;
        scene_hierarchy_layer& _scene_hierarchy_layer;
    };

}



