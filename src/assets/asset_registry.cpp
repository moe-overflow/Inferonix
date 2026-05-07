#include "asset_registry.hpp"

#include <entt/entt.hpp>

using namespace inferonix::asset;


asset_registry::asset_registry(registry& registry) : _registry(registry)
{
    _registry.ctx().emplace<asset_registry*>(this);
}



