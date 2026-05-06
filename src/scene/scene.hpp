#pragma once

#include "camera.hpp"
#include "editor_camera.hpp"
#include "../assets/asset_registry.hpp"
#include "entt/entt.hpp"
#include <memory>

namespace Inferonix::Scene
{
    using Registry = entt::registry;
    using Entity = entt::entity;

    class scene
    {
    public:
        scene();
        ~scene() = default;

        [[nodiscard]] Registry& GetRegistry() { return *_registry; }
        [[nodiscard]] const Registry& GetRegistry() const { return *_registry; }

        [[nodiscard]] Asset::asset_registry& GetAssetRegistry() { return *_asset_registry; }

        [[nodiscard]] std::shared_ptr<camera> GetEditorCamera() const { return _editor_camera; }

    private:
        std::unique_ptr<Registry> _registry;
        std::unique_ptr<Asset::asset_registry> _asset_registry;

        std::shared_ptr<editor_camera> _editor_camera;


    };
}