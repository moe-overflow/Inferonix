#pragma once

#include "../renderer/mesh.hpp"
#include "../renderer/transform.hpp"
#include "camera.hpp"
#include "../script/script_object.hpp"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>

namespace inferonix::scene
{

    using mesh_component = renderer::mesh;
    
    struct transform_component : public renderer::transform
    {
        transform_component() : renderer::transform() {}
        
        transform& get_transform() { return *this; }
        [[nodiscard]] const transform& get_transform() const { return *this; }
    };

    struct shader_component
    {
        std::string vertex_shader_path, fragment_shader_path;
    };

    using camera_component = camera;

    struct script_component
    {
        std::string script_path;
        script::script_object script_object;
        bool initialized {false};
    };

    enum class RigidBodyType { Static, Dynamic, Kinematic };

    struct rigid_body_component
    {
        float mass = 1.0f;
        JPH::BodyID body_id {};
        RigidBodyType type;
    };

    struct box_collider_component
    {
        glm::vec3 HalfExtents = {0.5f, 0.5f, 0.5f};
    };

}

