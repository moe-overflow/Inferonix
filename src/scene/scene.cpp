#include "scene.hpp"

#include "components.hpp"

using namespace inferonix::scene;

scene::scene() :
    _registry(std::make_unique<registry>()),
    _asset_registry(std::make_unique<asset::asset_registry>(*_registry))
{
    camera_settings camera_settings;
    camera_settings.is_primary = true;
    camera_settings.position = glm::vec3{1.0f, 1.0f, 1.0f};
    _editor_camera = std::make_shared<editor_camera>(camera_settings);
}


