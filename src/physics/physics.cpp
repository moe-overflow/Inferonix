#include "physics.hpp"

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

#include <memory>
#include "../scene/components.hpp"

using namespace Inferonix::Physics;
using namespace Inferonix::Scene;

PhysicsEngine::~PhysicsEngine()
{
    delete JPH::Factory::sInstance;
    JPH::UnregisterTypes();
}

namespace
{
    namespace Layers
    {
        constexpr JPH::ObjectLayer NON_MOVING = 0;
        constexpr JPH::ObjectLayer MOVING = 1;
        constexpr JPH::ObjectLayer NUM_LAYERS = 2;
    };

    namespace BroadPhaseLayers
    {
        constexpr JPH::BroadPhaseLayer NON_MOVING(0);
        constexpr JPH::BroadPhaseLayer MOVING(1);
        constexpr uint32_t NUM_LAYERS(2);
    };

    class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
    {
    public:
        BPLayerInterfaceImpl()
        {
            mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
            mObjectToBroadPhase[Layers::MOVING]     = BroadPhaseLayers::MOVING;
        }

        [[nodiscard]] uint32_t GetNumBroadPhaseLayers() const override { return BroadPhaseLayers::NUM_LAYERS; }

        [[nodiscard]] JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer in_layer) const override
        {
            return mObjectToBroadPhase[in_layer];
        }
    private:
        JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
    };

    class ObjectVsBroadPhaseLayerFilterImpl final : public JPH::ObjectVsBroadPhaseLayerFilter
    {
    public:
        [[nodiscard]] bool ShouldCollide(const JPH::ObjectLayer in_layer1, const JPH::BroadPhaseLayer in_layer2) const override {
            switch (in_layer1) {
                case Layers::NON_MOVING: return in_layer2 == BroadPhaseLayers::MOVING;
                case Layers::MOVING:     return true;
                default: return false;
            }
        }
    };

    class ObjectLayerPairFilterImpl final : public JPH::ObjectLayerPairFilter
    {
    public:
        [[nodiscard]] bool ShouldCollide(JPH::ObjectLayer in_object1, JPH::ObjectLayer in_object2) const override {
            switch (in_object1) {
                case Layers::NON_MOVING: return in_object2 == Layers::MOVING;
                case Layers::MOVING:     return true;
                default: return false;
            }
        }
    };

    BPLayerInterfaceImpl               g_bp_layer_interface;
    ObjectVsBroadPhaseLayerFilterImpl  g_obj_vs_bp_filter;
    ObjectLayerPairFilterImpl          g_obj_pair_filter;
}

void PhysicsEngine::Init()
{

    spdlog::info("Initializing Physics Engine");

    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    _temp_allocator.reset(new JPH::TempAllocatorImpl(10 * 1024 * 1024));
    _job_system.reset(new JPH::JobSystemThreadPool(
            JPH::cMaxPhysicsJobs,
            JPH::cMaxPhysicsBarriers,
            std::thread::hardware_concurrency() - 1
    ));

    _bp_layer_interface.reset(new BPLayerInterfaceImpl());
    _obj_vs_bp_filter.reset(new ObjectVsBroadPhaseLayerFilterImpl());
    _obj_pair_filter.reset(new ObjectLayerPairFilterImpl());

    _physics_system.reset(new JPH::PhysicsSystem());

    _physics_system->Init(
        1024, 0, 1024, 1024,
        *_bp_layer_interface,
        *_obj_vs_bp_filter,
        *_obj_pair_filter
    );

    _physics_system->SetGravity(JPH::Vec3(0.0f, -9.81f, 0.0f));

    spdlog::info("Initializing Physics Engine - done");
}

void PhysicsEngine::StartSimulation(Registry& registry) const
{
    auto& body_interface = _physics_system->GetBodyInterface();

    // Iterate through all entities that have physics components
    for (auto const view = registry.view<RigidBodyComponent, TransformComponent, BoxColliderComponent>();
         auto const entity : view)
    {
        auto& rigid_body_component = view.get<RigidBodyComponent>(entity);
        auto const& transform_component = view.get<TransformComponent>(entity);
        auto& [HalfExtents]  = view.get<BoxColliderComponent>(entity);

        // create a Jolt Box Shape using the Half Extents from our Collider component
        JPH::ObjectLayer const layer = (rigid_body_component.type == RigidBodyType::Static) ? Layers::NON_MOVING : Layers::MOVING;
        auto const motion_type = rigid_body_component.type == RigidBodyType::Static ? JPH::EMotionType::Static : JPH::EMotionType::Dynamic;

        JPH::Ref<JPH::Shape> shape = JPH::BoxShapeSettings{
            JPH::Vec3(HalfExtents.x, HalfExtents.y, HalfExtents.z)
        }.Create().Get();

        auto body_settings = JPH::BodyCreationSettings
        {
            shape,
            JPH::RVec3(transform_component.position.x, transform_component.position.y, transform_component.position.z),
            JPH::Quat::sIdentity(),
            motion_type,
            layer
        };

        JPH::Body* const body = body_interface.CreateBody(body_settings);
        rigid_body_component.body_id = body->GetID();
        body_interface.AddBody(body->GetID(), JPH::EActivation::Activate);
    }
}

void PhysicsEngine::Update(Registry& registry, float const delta_time) const
{
    constexpr int collision_steps = 1;
    _physics_system->Update(delta_time, collision_steps, _temp_allocator.get(), _job_system.get());

    // yield results to ECS
    auto const& body_interface = _physics_system->GetBodyInterface();
    for (auto const view = registry.view<RigidBodyComponent, TransformComponent>();
         auto const entity : view )
    {
        auto& rigid_body = view.get<RigidBodyComponent>(entity);
        auto& transform = view.get<TransformComponent>(entity);
        if (rigid_body.type == RigidBodyType::Dynamic)
        {
            JPH::Vec3 jolt_pos = body_interface.GetPosition(rigid_body.body_id);
            transform.position = { jolt_pos.GetX(), jolt_pos.GetY(), jolt_pos.GetZ() };
        }
    }

}