#include "scene.hpp"

#include "components.hpp"

using namespace Inferonix::Scene;

scene::scene() :
    _registry(std::make_unique<Registry>()),
    _asset_registry(std::make_unique<Asset::asset_registry>(*_registry))
{
    CameraSettings camera_settings;
    camera_settings.is_primary = true;
    camera_settings.position = glm::vec3(1.0f, 1.0f, 1.0f);
    _editor_camera = std::make_shared<editor_camera>(camera_settings);
}


