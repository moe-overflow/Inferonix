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

        [[nodiscard]] const entt::registry& GetRegistry() const
        {
            return *registry;
        }

    private:
        std::unique_ptr<entt::registry> registry;

    };
}