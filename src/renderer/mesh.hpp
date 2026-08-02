#pragma once

#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

namespace inferonix::renderer
{
    struct vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texture_coordinates;
    };

    class mesh
    {

    public:
        mesh() = default;

        explicit mesh(std::string const& id, std::string const& path) : _id(id)
        {
            load_from_file(path);
        }

        bool load_from_file(std::filesystem::path const& path)
        {
            LOG(LOG_TYPE::INFO, "Loading Mesh {}", path.string());

            auto importer = Assimp::Importer{};
            constexpr uint32_t flags = { aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices };

            auto* const scene = importer.ReadFile(path, flags);

            if (!scene || !scene->mRootNode)
            {
                LOG(LOG_TYPE::ERROR, "Error while loading model: {}", importer.GetErrorString());
                return false;
            }

            auto const mesh = scene->mMeshes[0];

            for (uint64_t i{ 0 }; i < mesh->mNumVertices; i++)
            {
                auto vertex_ = vertex{};
                vertex_.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

                if (mesh->mNormals)
                    vertex_.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
                else
                    vertex_.normal = { 0.0f, 1.0f, 0.0f };

                if (mesh->mTextureCoords[0])
                    vertex_.texture_coordinates = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
                else
                    vertex_.texture_coordinates = { 0.0f, 0.0f };

                _vertices.push_back(vertex_);
            }
            LOG(LOG_TYPE::INFO, "Number of vertices: {}", mesh->mNumVertices);

            for (uint64_t i{ 0 }; i < mesh->mNumFaces; i++)
            {
                auto const face = mesh->mFaces[i];
                for (uint64_t j{ 0 }; j < face.mNumIndices; j++)
                    _indices.push_back(face.mIndices[j]);
            }
            LOG(LOG_TYPE::INFO, "Number of indices: {}", mesh->mNumFaces);
            return true;
        }

        [[nodiscard]] std::vector<vertex> const& GetVertices()
        {
            return _vertices;
        }

        [[nodiscard]] std::vector<uint32_t>& GetIndices()
        {
            return _indices;
        }


    private:
        std::string _id;
        std::vector<vertex> _vertices;
        std::vector<uint32_t> _indices;
    };

}
