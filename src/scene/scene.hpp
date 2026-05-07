#pragma once

#include "camera.hpp"
#include "editor_camera.hpp"
#include "../assets/asset_registry.hpp"
#include "entt/entt.hpp"
#include <memory>

namespace inferonix::scene
{
    using registry = entt::registry;
    using entity = entt::entity;

    class scene
    {
    public:
        scene();
        ~scene() = default;

        [[nodiscard]] registry& get_registry() { return *_registry; }
        [[nodiscard]] const registry& get_registry() const { return *_registry; }

        [[nodiscard]] asset::asset_registry& get_asset_registry() const { return *_asset_registry; }

        [[nodiscard]] std::shared_ptr<camera> get_editor_camera() const { return _editor_camera; }

    private:
        std::unique_ptr<registry> _registry;
        std::unique_ptr<asset::asset_registry> _asset_registry;

        std::shared_ptr<editor_camera> _editor_camera;


    };
}