#include "Scene.hpp"

#include "Components.hpp"

using namespace Inferonix::Scene;

Scene::Scene() :
    _registry(std::make_unique<Registry>()),
    _asset_registry(std::make_unique<Asset::AssetRegistry>(*_registry))
{
    CameraSettings camera_settings;
    camera_settings.is_primary = true;
    camera_settings.position = glm::vec3(1.0f, 1.0f, 1.0f);
    _main_camera = std::make_shared<Camera>(camera_settings);
}


