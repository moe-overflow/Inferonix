#pragma once

#include "Asset/AssetRegistry.hpp"
#include "entt/entt.hpp"
#include <memory>

namespace Inferonix::Scene
{
    using Registry = entt::registry;
    using Entity = entt::entity;

    class Scene
    {
    public:
        Scene();
        ~Scene() = default;



        [[nodiscard]] Registry& GetRegistry() { return *_registry; }

        [[nodiscard]] const Registry& GetRegistry() const
        {
            return *_registry;
        }

    private:
        std::unique_ptr<Registry> _registry;

        std::unique_ptr<Asset::AssetRegistry> _asset_registry;

    };
}