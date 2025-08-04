#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

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
        explicit Mesh(std::string const& path)
        {
            spdlog::info("Loading Mesh {}", path);

            Assimp::Importer importer{};

            constexpr uint32_t flags = { aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices };

            auto* const scene = importer.ReadFile(path, flags);

            if (!scene || !scene->mRootNode)
                throw std::runtime_error(fmt::format("Error while loading model: {}", importer.GetErrorString()));

            aiMesh* mesh = scene->mMeshes[0];

            for (uint64_t i{ 0 }; i < mesh->mNumVertices; i++)
            {
                Vertex vertex{
                    .Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z },
                    .Normal = { mesh->mNormals[i].x,  mesh->mNormals[i].y,  mesh->mNormals[i].z }
                };
                _vertices.push_back(vertex);
            }
            spdlog::info("Number of vertices: {}", mesh->mNumVertices);


            for (uint64_t i{ 0 }; i < mesh->mNumFaces; i++)
            {
                aiFace const face = mesh->mFaces[i];
                for (uint64_t j{ 0 }; j < face.mNumIndices; j++)
                    _indices.push_back(face.mIndices[j]);
            }
            spdlog::info("Number of indices: {}", mesh->mNumFaces);
        }

        [[nodiscard]] std::vector<Vertex> const& GetVertices()
        {
            return _vertices;
        }

        [[nodiscard]] std::vector<unsigned int> const& GetIndices()
        {
            return _indices;
        }


    private:
        std::vector<Vertex> _vertices;
        std::vector<unsigned int> _indices;
    };

} // namespace Inferonix::Renderer
