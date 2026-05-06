#include "asset_registry.hpp"

#include <entt/entt.hpp>

using namespace Inferonix::Asset;


asset_registry::asset_registry(Registry& registry) : _registry(registry)
{
    _registry.ctx().emplace<asset_registry*>(this);
}



