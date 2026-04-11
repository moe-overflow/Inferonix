#pragma once

#include "Camera.hpp"
#include "EditorCamera.hpp"
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

        [[nodiscard]] std::shared_ptr<Camera> GetEditorCamera() const { return _editor_camera; }

    private:
        std::unique_ptr<Registry> _registry;
        std::unique_ptr<Asset::AssetRegistry> _asset_registry;

        std::shared_ptr<EditorCamera> _editor_camera;


    };
}