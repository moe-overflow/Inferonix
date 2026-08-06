#pragma once

#include "camera.hpp"
#include "editor_camera.hpp"
#include "assets/asset_registry.hpp"

#include <entt/entt.hpp>
#include <memory>

namespace inferonix::scene
{
    using registry = entt::registry;
    using entity = entt::entity;

    class scene
    {
    public:
        scene() :
            _registry(std::make_unique<registry>()),
            _asset_registry(std::make_unique<asset::asset_registry>(*_registry))
        {
            camera_settings camera_settings;
            camera_settings.is_primary = true;
            camera_settings.position = glm::vec3{0.0f, 1.0f, 5.0f};
            _editor_camera = std::make_shared<editor_camera>(camera_settings);
        }

        ~scene() = default;

        [[nodiscard]] registry& get_registry() { return *_registry; }
        [[nodiscard]] const registry& get_registry() const { return *_registry; }

        [[nodiscard]] asset::asset_registry& get_asset_registry() const { return *_asset_registry; }
        [[nodiscard]] std::shared_ptr<camera> get_editor_camera() const { return _editor_camera; }

    private:
        // responsible for holding scene data (e.g. entities and their components)
        std::unique_ptr<registry> _registry;

        // responsible for loading assets files
        std::unique_ptr<asset::asset_registry> _asset_registry;

        std::shared_ptr<editor_camera> _editor_camera;


    };
}