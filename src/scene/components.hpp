#pragma once

#include <string>
#include <memory>

#include "../renderer/mesh.hpp"
#include "../renderer/transform.hpp"
#include "../renderer/texture.hpp"
#include "camera.hpp"
#include "../script/script_object.hpp"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>

#include "util/uuid.hpp"

namespace inferonix::scene
{
    // This is needed to each scene object gets an UUID that is fixed rather than using
    //  an entt::entity handle that is not consistent when reloading the engine/game
    struct id_component
    {
        std::string id;
        id_component() = default;
        id_component(const id_component& id) = default;
        id_component(const std::string& id) : id(id) {}
    };

    // human readable ID of an object
    struct tag_component
    {
        std::string tag;
        tag_component() = default;
        tag_component(const tag_component&) = default;
        tag_component(const std::string& tag) : tag(tag) {}
    };

    using mesh_component = renderer::mesh;

    struct material_component
    {
        glm::vec3 color { .0f, .0f, .0f };
        std::shared_ptr<renderer::texture> albedo_map;
        bool use_texture { false };
        bool use_dynamic_color { false };
    };

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
        glm::vec3 half_extents = {0.5f, 0.5f, 0.5f};
    };

}

