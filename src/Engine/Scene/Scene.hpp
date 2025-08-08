#pragma once

#include "Camera.hpp"
#include "../Asset/AssetRegistry.hpp"
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
        [[nodiscard]] const Registry& GetRegistry() const { return *_registry; }

        [[nodiscard]] Asset::AssetRegistry& GetAssetRegistry() { return *_asset_registry; }

        [[nodiscard]] std::shared_ptr<Camera> GetMainCamera() const { return _main_camera; }

    private:
        std::unique_ptr<Registry> _registry;
        std::unique_ptr<Asset::AssetRegistry> _asset_registry;

        std::shared_ptr<Camera> _main_camera;

        // std::vector<std::shared_ptr<Camera>> _secondary_cameras;

    };
}