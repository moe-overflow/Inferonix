#include "AssetRegistry.hpp"

#include <entt/entt.hpp>

using namespace Inferonix::Asset;


AssetRegistry::AssetRegistry(Registry& registry) : _registry(registry)
{
    _registry.ctx().emplace<AssetRegistry*>(this);
}



