#pragma once

#include "../Renderer/Mesh.hpp"
#include "../Renderer/Transform.hpp"
#include "Camera.hpp"
#include "../Script/ScriptObject.hpp"

namespace Inferonix::Scene
{

    using MeshComponent = Renderer::Mesh;
    
    struct TransformComponent : public Renderer::Transform
    {
        TransformComponent() : Renderer::Transform() {}
        
        Transform& GetTransform() { return *this; }
        [[nodiscard]] const Transform& GetTransform() const { return *this; }
    };

    struct ShaderComponent
    {
        std::string vertex_shader_path, fragment_shader_path;
    };

    using CameraComponent = Camera;

    struct ScriptComponent
    {
        std::string script_path;
        Script::ScriptObject script_object;
        bool initialized {false};
    };

}

