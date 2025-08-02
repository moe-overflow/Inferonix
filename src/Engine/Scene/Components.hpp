#pragma once

#include "../Renderer/Mesh.hpp"
#include "../Renderer/Transform.hpp"

#include <memory>

namespace Inferonix::Scene
{
    struct MeshComponent
    {
        std::shared_ptr<Renderer::Mesh> mesh_instance;
        bool dynamically_colored;
    };

    using TransformComponent = Renderer::Transform;

}

