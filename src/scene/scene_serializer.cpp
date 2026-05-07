#include "scene_serializer.hpp"

#include "components.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

using namespace inferonix::scene;

scene_serializer::scene_serializer(scene& scene) : _scene(scene) {}

bool scene_serializer::Serialize(const std::string& filepath) const
{
    // todo
    return false;
}

bool scene_serializer::Deserialize(const std::string& filepath) const
{
    std::ifstream file(filepath);
    auto data = nlohmann::json::parse(file);

    auto& registry = _scene.get_registry();
    auto& assets = _scene.get_asset_registry();

    for (auto& entity_data : data["entities"])
    {
        auto entity = registry.create();
        if (entity_data["Components"].contains("TransformComponent"))
        {
            auto& transform = entity_data["Components"]["TransformComponent"];
            auto tc = transform_component{};
            tc.position = {transform["Position"][0], transform["Position"][1], transform["Position"][2]};
            tc.scale = {transform["Scale"][0], transform["Scale"][1], transform["Scale"][2]};
            registry.emplace<transform_component>(entity, tc);
        }

        if (entity_data["Components"].contains("MeshComponent"))
        {
            auto& mesh_data = entity_data["Components"]["MeshComponent"];
            auto path = mesh_data["AssetPath"];
            auto id = mesh_data["AssetID"];

            auto mesh = assets.load<renderer::mesh>(id, path);
            if (mesh)
                registry.emplace<mesh_component>(entity, *mesh.value());
        }

        if (entity_data["Components"].contains("ScriptComponent"))
        {
            auto& script_data = entity_data["Components"]["ScriptComponent"];
            auto script = script_component{};
            script.script_path = script_data["ScriptPath"];
            script.initialized = false;
            registry.emplace<script_component>(entity, std::move(script));
        }

    }

    return true;

}


