#pragma once

#include "../Renderer/Mesh.hpp"
#include "../Renderer/Transform.hpp"
#include "Camera.hpp"
#include "../Script/ScriptObject.hpp"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>

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

    enum class RigidBodyType { Static, Dynamic, Kinematic };

    struct RigidBodyComponent
    {
        float mass = 1.0f;
        JPH::BodyID body_id {};
        RigidBodyType type;
    };

    struct BoxColliderComponent
    {
        glm::vec3 HalfExtents = {0.5f, 0.5f, 0.5f};
    };

}

