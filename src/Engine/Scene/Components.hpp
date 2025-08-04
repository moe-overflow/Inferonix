#pragma once

#include "../Renderer/Mesh.hpp"
#include "../Renderer/Transform.hpp"
#include "Camera.hpp"

namespace Inferonix::Scene
{

    using MeshComponent = Renderer::Mesh;
    using TransformComponent = Renderer::Transform;

    struct ShaderComponent
    {
        std::string vertex_shader_path, fragment_shader_path;
    };

    using CameraComponent = Camera;

}

