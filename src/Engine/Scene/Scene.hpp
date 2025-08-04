#pragma once

#include <memory>

#include "entt/entt.hpp"

namespace Inferonix::Scene
{

    class Scene
    {
    public:
        Scene() = default;

        ~Scene() = default;

        [[nodiscard]] entt::registry& GetRegistry() { return *_registry; }

        [[nodiscard]] const entt::registry& GetRegistry() const
        {
            return *_registry;
        }

    private:
        std::unique_ptr<entt::registry> _registry;

    };
}