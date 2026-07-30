#pragma once

#include <imgui.h>

#include "layer.hpp"

namespace inferonix::ui
{
    class scene_hierarchy final : public layer
    {
    public:
        explicit scene_hierarchy(entt::registry& registry) : _registry(registry)
        {}


        void on_render() override
        {
            using namespace ImGui;
            Begin("Scene Hierarchy", nullptr);

            for (auto entity : _registry.storage<entt::entity>())
            {
                auto name = std::string{};
                if (_registry.all_of<scene::tag_component>(entity))
                    name = _registry.get<scene::tag_component>(entity).tag;
                else
                    name = "Entity " + std::to_string(static_cast<uint32_t>(entity));

                bool is_selected = (_selected_entity == entity);

                if (Selectable(name.c_str(), is_selected))
                    _selected_entity = entity;

            }
            End();
        }

        [[nodiscard]] auto get_selected_entity() const -> entt::entity { return _selected_entity; }
        void set_selected_entity(const entt::entity entity) { _selected_entity = entity; }
    
    private:
        entt::registry& _registry;
        entt::entity _selected_entity { entt::null };

    };
}

