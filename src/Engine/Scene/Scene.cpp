#include "Scene.hpp"

#include "Components.hpp"

using namespace Inferonix::Scene;

Scene::Scene() :
    _registry(std::make_unique<Registry>()),
    _asset_registry(std::make_unique<Asset::AssetRegistry>(*_registry))
{
    // Monkey
    {
        auto const monkey = _registry->create();

        auto transform = _registry->emplace<TransformComponent>(monkey);
        transform.position = glm::vec3{2, 100, 50};

        auto shader = _registry->emplace<ShaderComponent>(monkey);

        auto mesh = _registry->emplace<MeshComponent>(monkey, RESOURCES_PATH "models/Monkey.obj");

        // auto camera = _registry->emplace<CameraComponent>(monkey, transform, shader);
        auto camera = _registry->create();
        _registry->emplace<TransformComponent>(camera);
        _registry->emplace<CameraComponent>(camera);
    }
}


