#include "Scene.hpp"

#include "Components.hpp"

using namespace Inferonix::Scene;

Scene::Scene() :
    _registry(std::make_unique<Registry>()),
    _asset_registry(std::make_unique<Asset::AssetRegistry>(*_registry))
{}


