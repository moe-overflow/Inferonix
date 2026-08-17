#pragma once

#include <string>
#include <memory>

#include "math/transform.hpp"
#include "renderer/texture.hpp"
#include "scene/model.hpp"
#include "camera.hpp"
#include "renderer/compute_shader.hpp"
#include "renderer/shader_storage_buffer.hpp"
#include "script/script_object.hpp"

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

    struct mesh_component
    {
        std::shared_ptr<model> model_asset;
    };


    struct material_component
    {
        glm::vec3 color { .0f, .0f, .0f };
        std::shared_ptr<renderer::texture> albedo_map;
        bool use_texture { false };
        bool use_dynamic_color { false };
    };

    struct transform_component : public math::transform
    {
        transform_component() : math::transform() {}
        
        math::transform& get_transform() { return *this; }
        [[nodiscard]] const math::transform& get_transform() const { return *this; }
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

    struct simulation_component
    {
        struct alignas(16) particle final {
            glm::vec4 position{0.0f};
            glm::vec4 velocity{0.0f};
            glm::vec4 age_lifetime{0.0f, 5.0f, 0.0f, 0.0f};
        };
        std::vector<particle> particles;
        renderer::shader_storage_buffer storage_buffer;
        uint32_t workgroup_size{ 64 };
        uint32_t binding_point{ 0 };
        float drag{ 1.5f };
    };

}

