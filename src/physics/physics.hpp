#pragma once

#include "../scene/scene.hpp"
#include "../scene/components.hpp"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Core/JobSystem.h>
#include <Jolt/Physics/PhysicsSystem.h>

namespace JPH { class PhysicsSystem; class JobSystem; class TempAllocator; }
namespace Inferonix::Physics
{
    class PhysicsEngine final
    {
    public:
        PhysicsEngine() = default;
        ~PhysicsEngine();

        void Init();
        void StartSimulation(Scene::Registry& registry) const;

        void Update(Scene::Registry& registry, float delta_time) const;

    private:
        std::unique_ptr<JPH::PhysicsSystem> _physics_system;
        std::unique_ptr<JPH::JobSystem> _job_system;
        std::unique_ptr<JPH::TempAllocator> _temp_allocator;

        std::unique_ptr<JPH::BroadPhaseLayerInterface> _bp_layer_interface;
        std::unique_ptr<JPH::ObjectVsBroadPhaseLayerFilter> _obj_vs_bp_filter;
        std::unique_ptr<JPH::ObjectLayerPairFilter> _obj_pair_filter;
    };
}