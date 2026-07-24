#include "scene_serializer.hpp"

#include "components.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

using namespace inferonix::scene;

scene_serializer::scene_serializer(scene& scene) : _scene(scene) {}

bool scene_serializer::serialize(const std::string& filepath) const
{
    // todo
    return false;
}

void scene_serializer::deserialize(const std::string& filepath) const
{
    assert(std::filesystem::exists(filepath));
    auto file = std::ifstream{filepath};
    auto data = nlohmann::json::parse(file);

    auto& registry = _scene.get_registry();
    auto& assets = _scene.get_asset_registry();

    for (auto& entity_data : data["Entities"])
    {
        auto entity = registry.create();
        auto name = entity_data.contains("Name") ? entity_data["Name"] : "Unnamed";
        registry.emplace<tag_component>(entity, name);
        registry.emplace<id_component>(entity, utils::generate_uuid());

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
            auto path = mesh_data["AssetPath"].get<std::string>();
            auto id = mesh_data["AssetID"];

            auto mesh = assets.load<renderer::mesh>(id, path);
            if (mesh)
                registry.emplace<mesh_component>(entity, *mesh.value());

            auto material = material_component{};
            if (mesh_data.contains("TexturePath") && mesh_data.contains("TextureID"))
            {
                auto tex_path =  mesh_data["TexturePath"].get<std::string>();
                auto tex_id = mesh_data["TextureID"];
                auto texture = assets.load<renderer::texture>(tex_id, tex_path);

                if (texture)
                {
                    material.albedo_map = texture.value();
                    material.use_texture = true;
                }
            }
            else if (mesh_data.contains("Color"))
            {
                if (auto& c = mesh_data["Color"]; c == "DynamicColor" )
                {
                    material.use_dynamic_color = true;
                }
                else // TODO: ensure color is valid
                {
                    material.color = glm::vec3{ c[0], c[1], c[2] };
                    material.use_texture = false;
                }
            }
            registry.emplace<material_component>(entity, material);
        }

        if (entity_data["Components"].contains("ScriptComponent"))
        {
            auto& script_data = entity_data["Components"]["ScriptComponent"];
            auto script = script_component{};
            script.script_path = script_data["ScriptPath"].get<std::string>();
            script.initialized = false;
            registry.emplace<script_component>(entity, std::move(script));
        }

    }
}


