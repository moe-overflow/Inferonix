#pragma once

#include "../renderer/mesh.hpp"
#include "../renderer/transform.hpp"
#include "camera.hpp"
#include "../script/script_object.hpp"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>

namespace Inferonix::Scene
{

    using MeshComponent = Renderer::mesh;
    
    struct TransformComponent : public Renderer::transform
    {
        TransformComponent() : Renderer::transform() {}
        
        transform& GetTransform() { return *this; }
        [[nodiscard]] const transform& GetTransform() const { return *this; }
    };

    struct ShaderComponent
    {
        std::string vertex_shader_path, fragment_shader_path;
    };

    using CameraComponent = camera;

    struct ScriptComponent
    {
        std::string script_path;
        Script::script_object script_object;
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

