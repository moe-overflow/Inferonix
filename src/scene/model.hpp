#pragma once

#include "util/logger.hpp"
#include "renderer/vertex.hpp"
#include "renderer/color.hpp"
#include "math/transform.hpp"
#include "assets/asset_registry.hpp"

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace inferonix::scene
{
    struct mesh
    {
        std::vector<renderer::vertex> _vertices;
        std::vector<uint32_t> _indices;
        uint32_t material_index = 0;
    };

    struct model_node
    {
        std::string name;
        math::transform local_transform;
        std::vector<uint32_t> mesh_indices;
        std::vector<model_node> children;
    };

    struct material
    {
        renderer::color base_color{1.f, 1.f, 1.f, 1.f};
        std::shared_ptr<renderer::texture> albedo_texture;
        float metallic = 1.0f;
        float roughness = 1.0f;
    };

    class model final
    {
    public:
        model() = default;
        ~model() = default;
        model(const model&) = delete;
        model& operator=(const model&) = delete;
        model(model&&) = default;
        model& operator=(model&&) = default;

        auto load(std::filesystem::path const& path, asset::asset_registry& registry) -> bool
        {
            _model_directory = path.parent_path();
            LOG(LOG_TYPE::INFO, "Loading Mesh {}", path.string());

            auto importer = Assimp::Importer{};
            constexpr uint32_t flags = {
                aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices
            };

            auto* const scene = importer.ReadFile(path, flags);

            if (!scene || !scene->mRootNode)
            {
                LOG(LOG_TYPE::WARNING, "Error while loading model: {}", importer.GetErrorString());
                return false;
            }

            register_node(scene->mRootNode, _root);

            for (size_t i = 0; i < scene->mNumMeshes; ++i)
                register_mesh(scene->mMeshes[i]);

            for (size_t i = 0; i < scene->mNumMaterials; ++i)
                register_material(scene, scene->mMaterials[i], registry);

            return true;
        }

        [[nodiscard]] const std::vector<mesh>& meshes() const
        {
            return _meshes;
        }

        [[nodiscard]]
        const std::vector<material>& materials() const
        {
            return _materials;
        }

    private:
        auto register_node(aiNode* node, model_node& output) -> void
        {
#if 0
            output.name = node->mName.C_Str();

            output.local_transform.set_from_matrix(ai_to_glm(node->mTransformation));

            for (unsigned i = 0; i < node->mNumMeshes; ++i)
                output.mesh_indices.push_back(node->mMeshes[i]);

            for (unsigned i = 0; i < node->mNumChildren; ++i)
            {
                output.children.emplace_back();
                register_node(node->mChildren[i], output.children.back());
            }
#endif
        }

        auto register_material(const aiScene* scene, const aiMaterial* input_material, asset::asset_registry& registry) -> void
        {
            auto result_material = material{};
            auto color = aiColor4D{ .0f, .0f, .0f, 1.0f };

            if (aiGetMaterialColor(input_material, AI_MATKEY_BASE_COLOR, &color) == AI_SUCCESS)
                result_material.base_color = renderer::color{ color.r, color.g, color.b, color.a };
            else if (aiGetMaterialColor(input_material, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS)
                result_material.base_color = renderer::color{ color.r, color.g, color.b, color.a };

            ai_real metallic;
            if (aiGetMaterialFloat(input_material, AI_MATKEY_METALLIC_FACTOR, &metallic) == AI_SUCCESS)
                result_material.metallic = metallic;


            ai_real roughness;
            if (aiGetMaterialFloat(input_material, AI_MATKEY_ROUGHNESS_FACTOR, &roughness) == AI_SUCCESS)
                result_material.roughness = roughness;

            aiString texPath;
            if (input_material->GetTexture(aiTextureType_BASE_COLOR, 0, &texPath) == AI_SUCCESS)
            {
                const auto path = std::string{texPath.C_Str()};
                const auto full_path = _model_directory / path;
                if (!path.empty() && path[0] == '*')
                {
                    int index = std::stoi(path.substr(1));
                    auto tex = std::make_shared<renderer::texture>();
                    if (tex->load_from_assimp(scene->mTextures[index]))
                        result_material.albedo_texture = tex;
                }
                else
                {
                    auto texture = registry.load<renderer::texture>(full_path.string(), full_path);
                    if (texture)
                        result_material.albedo_texture = *texture;
                }
            }

            aiString tex;
            if (input_material->GetTexture(aiTextureType_BASE_COLOR, 0, &tex) == AI_SUCCESS)
            {
                LOG(LOG_TYPE::INFO, "Base color texture = '{}'", tex.C_Str());
            }
            else
            {
                LOG(LOG_TYPE::INFO, "No base color texture");
            }

            _materials.emplace_back(std::move(result_material));
        }

        auto register_mesh(aiMesh* input_mesh) -> void
        {
            auto result_mesh = mesh{};
            for (uint64_t i{ 0 }; i < input_mesh->mNumVertices; i++)
            {
                auto vertex_ = renderer::vertex{};
                vertex_.position = { input_mesh->mVertices[i].x, input_mesh->mVertices[i].y, input_mesh->mVertices[i].z };

                if (input_mesh->mNormals)
                    vertex_.normal = { input_mesh->mNormals[i].x, input_mesh->mNormals[i].y, input_mesh->mNormals[i].z };
                else
                    vertex_.normal = { 0.0f, 1.0f, 0.0f };

                if (input_mesh->mTextureCoords[0])
                    vertex_.texture_coordinates = { input_mesh->mTextureCoords[0][i].x, input_mesh->mTextureCoords[0][i].y };
                else
                    vertex_.texture_coordinates = { 0.0f, 0.0f };

                result_mesh._vertices.push_back(vertex_);
            }
            LOG(LOG_TYPE::INFO, "Number of vertices: {}", input_mesh->mNumVertices);

            for (uint64_t i{ 0 }; i < input_mesh->mNumFaces; i++)
            {
                auto const face = input_mesh->mFaces[i];
                for (uint64_t j{ 0 }; j < face.mNumIndices; j++)
                    result_mesh._indices.push_back(face.mIndices[j]);
            }
            LOG(LOG_TYPE::INFO, "Number of indices: {}", input_mesh->mNumFaces);

            result_mesh.material_index = input_mesh->mMaterialIndex;
            LOG(LOG_TYPE::INFO, "Material index: {}", result_mesh.material_index);

            _meshes.emplace_back(result_mesh);
        }


    private:
        std::string _id;
        std::vector<mesh> _meshes;
        model_node _root;
        std::vector<material> _materials;
        std::filesystem::path _model_directory;


    };
}


