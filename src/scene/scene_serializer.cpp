#include "scene_serializer.hpp"

#include "components.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

using namespace Inferonix::Scene;

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

    auto& registry = _scene.GetRegistry();
    auto& assets = _scene.GetAssetRegistry();

    for (auto& entity_data : data["entities"])
    {
        auto entity = registry.create();
        if (entity_data["Components"].contains("TransformComponent"))
        {
            auto& transform = entity_data["Components"]["TransformComponent"];
            auto tc = TransformComponent{};
            tc.position = {transform["Position"][0], transform["Position"][1], transform["Position"][2]};
            tc.scale = {transform["Scale"][0], transform["Scale"][1], transform["Scale"][2]};
            registry.emplace<TransformComponent>(entity, tc);
        }

        if (entity_data["Components"].contains("MeshComponent"))
        {
            auto& mesh_data = entity_data["Components"]["MeshComponent"];
            auto path = mesh_data["AssetPath"];
            auto id = mesh_data["AssetID"];

            auto mesh = assets.Load<Renderer::mesh>(id, path);
            if (mesh)
                registry.emplace<MeshComponent>(entity, *mesh.value());
        }

        if (entity_data["Components"].contains("ScriptComponent"))
        {
            auto& script_data = entity_data["Components"]["ScriptComponent"];
            auto script = ScriptComponent{};
            script.script_path = script_data["ScriptPath"];
            script.initialized = false;
            registry.emplace<ScriptComponent>(entity, std::move(script));
        }

    }

    return true;

}


