#pragma once

#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

namespace Inferonix::Renderer
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        // TODO: TextureCoordinates here to add
    };

    class Mesh
    {

    public:
        Mesh() = default;

        explicit Mesh(std::string const& id, std::string const& path) : _id(id)
        {
            LoadFromFile(path);
        }

        bool LoadFromFile(std::filesystem::path const& path)
        {
            spdlog::info(fmt::format("Loading Mesh {}", path.string()));

            Assimp::Importer importer{};

            constexpr uint32_t flags = { aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices };

            auto* const scene = importer.ReadFile(path, flags);

            if (!scene || !scene->mRootNode)
            {
                spdlog::error(fmt::format("Error while loading model: {}", importer.GetErrorString()));
                return false;
            }

            auto const mesh = scene->mMeshes[0];

            for (uint64_t i{ 0 }; i < mesh->mNumVertices; i++)
            {
                Vertex vertex{
                    .Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z },
                    .Normal = { mesh->mNormals[i].x,  mesh->mNormals[i].y,  mesh->mNormals[i].z }
                };
                _vertices.push_back(vertex);
            }
            spdlog::info(fmt::format("Number of vertices: {}", mesh->mNumVertices));


            for (uint64_t i{ 0 }; i < mesh->mNumFaces; i++)
            {
                auto const face = mesh->mFaces[i];
                for (uint64_t j{ 0 }; j < face.mNumIndices; j++)
                    _indices.push_back(face.mIndices[j]);
            }
            spdlog::info(fmt::format("Number of indices: {}", mesh->mNumFaces));
            return true;
        }

        [[nodiscard]] std::vector<Vertex> const& GetVertices()
        {
            return _vertices;
        }

        [[nodiscard]] std::vector<uint32_t>& GetIndices()
        {
            return _indices;
        }


    private:
        std::string _id;
        std::vector<Vertex> _vertices;
        std::vector<uint32_t> _indices;
    };

}
