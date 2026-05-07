#include "physics.hpp"

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

#include <memory>
#include "../scene/components.hpp"

using namespace inferonix::physics;
using namespace inferonix::scene;

physics_engine::~physics_engine()
{
    delete JPH::Factory::sInstance;
    JPH::UnregisterTypes();
}

namespace
{
    namespace layers
    {
        constexpr JPH::ObjectLayer NON_MOVING = 0;
        constexpr JPH::ObjectLayer MOVING = 1;
        constexpr JPH::ObjectLayer NUM_LAYERS = 2;
    };

    namespace broad_phase_layers
    {
        constexpr JPH::BroadPhaseLayer NON_MOVING(0);
        constexpr JPH::BroadPhaseLayer MOVING(1);
        constexpr uint32_t NUM_LAYERS(2);
    };

    class bp_layer_interface_impl final : public JPH::BroadPhaseLayerInterface
    {
    public:
        bp_layer_interface_impl()
        {
            mObjectToBroadPhase[layers::NON_MOVING] = broad_phase_layers::NON_MOVING;
            mObjectToBroadPhase[layers::MOVING]     = broad_phase_layers::MOVING;
        }

        [[nodiscard]] uint32_t GetNumBroadPhaseLayers() const override { return broad_phase_layers::NUM_LAYERS; }

        [[nodiscard]] JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer in_layer) const override
        {
            return mObjectToBroadPhase[in_layer];
        }
    private:
        JPH::BroadPhaseLayer mObjectToBroadPhase[layers::NUM_LAYERS];
    };

    class object_vs_broad_phase_layer_filter_impl final : public JPH::ObjectVsBroadPhaseLayerFilter
    {
    public:
        [[nodiscard]] bool ShouldCollide(const JPH::ObjectLayer in_layer1, const JPH::BroadPhaseLayer in_layer2) const override {
            switch (in_layer1) {
                case layers::NON_MOVING: return in_layer2 == broad_phase_layers::MOVING;
                case layers::MOVING:     return true;
                default: return false;
            }
        }
    };

    class object_layer_pair_filter_impl final : public JPH::ObjectLayerPairFilter
    {
    public:
        [[nodiscard]] bool ShouldCollide(JPH::ObjectLayer in_object1, JPH::ObjectLayer in_object2) const override {
            switch (in_object1) {
                case layers::NON_MOVING: return in_object2 == layers::MOVING;
                case layers::MOVING:     return true;
                default: return false;
            }
        }
    };

    bp_layer_interface_impl               g_bp_layer_interface;
    object_vs_broad_phase_layer_filter_impl  g_obj_vs_bp_filter;
    object_layer_pair_filter_impl          g_obj_pair_filter;
}

void physics_engine::init()
{

    spdlog::info("Initializing Physics Engine");

    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    _temp_allocator = std::make_unique<JPH::TempAllocatorImpl>(10 * 1024 * 1024);
    _job_system = std::make_unique<JPH::JobSystemThreadPool>(
            JPH::cMaxPhysicsJobs,
            JPH::cMaxPhysicsBarriers,
            std::thread::hardware_concurrency() - 1
    );

    _bp_layer_interface = std::make_unique<bp_layer_interface_impl>();
    _obj_vs_bp_filter = std::make_unique<object_vs_broad_phase_layer_filter_impl>();
    _obj_pair_filter = std::make_unique<object_layer_pair_filter_impl>();

    _physics_system = std::make_unique<JPH::PhysicsSystem>();

    _physics_system->Init(
        1024, 0, 1024, 1024,
        *_bp_layer_interface,
        *_obj_vs_bp_filter,
        *_obj_pair_filter
    );

    _physics_system->SetGravity(JPH::Vec3(0.0f, -9.81f, 0.0f));

    spdlog::info("Initializing Physics Engine - done");
}

void physics_engine::start_simulation(registry& registry) const
{
    auto& body_interface = _physics_system->GetBodyInterface();

    // Iterate through all entities that have physics components
    auto const view = registry.view<rigid_body_component, transform_component, box_collider_component>();
    for (auto const entity_ : view)
    {
        auto& rigid_body_component_ = view.get<rigid_body_component>(entity_);
        auto const& transform_component_ = view.get<transform_component>(entity_);
        auto& [HalfExtents]  = view.get<box_collider_component>(entity_);

        // create a Jolt Box Shape using the Half Extents from our Collider component
        JPH::ObjectLayer const layer = (rigid_body_component_.type == RigidBodyType::Static) ? layers::NON_MOVING : layers::MOVING;
        auto const motion_type = rigid_body_component_.type == RigidBodyType::Static ? JPH::EMotionType::Static : JPH::EMotionType::Dynamic;

        JPH::Ref<JPH::Shape> shape = JPH::BoxShapeSettings{
            JPH::Vec3(HalfExtents.x, HalfExtents.y, HalfExtents.z)
        }.Create().Get();

        auto body_settings = JPH::BodyCreationSettings
        {
            shape,
            JPH::RVec3(transform_component_.position.x, transform_component_.position.y, transform_component_.position.z),
            JPH::Quat::sIdentity(),
            motion_type,
            layer
        };

        const JPH::Body* const body = body_interface.CreateBody(body_settings);
        rigid_body_component_.body_id = body->GetID();
        body_interface.AddBody(body->GetID(), JPH::EActivation::Activate);
    }
}

void physics_engine::update(registry& registry, float const delta_time) const
{
    constexpr int collision_steps = 1;
    _physics_system->Update(delta_time, collision_steps, _temp_allocator.get(), _job_system.get());

    // yield results to ECS
    auto const& body_interface = _physics_system->GetBodyInterface();
    for (auto const view = registry.view<rigid_body_component, transform_component>();
         auto const entity : view )
    {
        auto& rigid_body = view.get<rigid_body_component>(entity);
        auto& transform = view.get<transform_component>(entity);
        if (rigid_body.type == RigidBodyType::Dynamic)
        {
            JPH::Vec3 jolt_pos = body_interface.GetPosition(rigid_body.body_id);
            transform.position = { jolt_pos.GetX(), jolt_pos.GetY(), jolt_pos.GetZ() };
        }
    }

}