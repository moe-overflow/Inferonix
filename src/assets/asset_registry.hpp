#pragma once

#include <entt/entt.hpp>

#include <memory>
#include <any>
#include <expected>
#include <filesystem>
#include <variant>
#include <util/logger.hpp>

#include "renderer/texture.hpp"

namespace inferonix::scene{ class model; }
namespace inferonix::asset
{
    class asset_registry;
    using registry = entt::registry;
    using asset_handle = std::variant<std::shared_ptr<renderer::texture>, std::shared_ptr<scene::model>>;

    template<typename T>
    concept loadable_asset = requires(T t, const std::filesystem::path& path, asset_registry& registry)
    {
        { t.load(path, registry) } -> std::same_as<bool>;
    };

    class asset_registry final
    {
    public:
        explicit asset_registry(registry& registry) : _registry(registry)
        {
            _registry.ctx().emplace<asset_registry*>(this);
        }

        ~asset_registry() = default;

        asset_registry(const asset_registry&) = delete;
        asset_registry& operator=(const asset_registry&) = delete;

        template<loadable_asset asset_type>
        std::expected<std::shared_ptr<asset_type>, std::string> load(
            std::string const& id, std::filesystem::path const& path
        )
        {
            LOG(LOG_TYPE::INFO, "Attempting to load asset: {}", id);

            if (auto const it = _assets.find(id); it != _assets.end())
            {
                return std::get<std::shared_ptr<asset_type>>(it->second);
            }

            auto asset = std::make_shared<asset_type>();
            if (!asset->load(path, *this))
            {
                LOG(LOG_TYPE::WARNING, "Failed to load asset: {}", id);
                return std::unexpected(std::format("Failed to load asset {}", path.string()));
            }
            LOG(LOG_TYPE::INFO, "Loaded asset from {} successfully!", path.string());

            _assets.emplace(id, asset);
            return asset;

        }

        template<loadable_asset asset_type>
        std::shared_ptr<asset_type> get(const std::string& id)
        {
            auto const it = _assets.find(id);
            if (it == _assets.end())
                return nullptr;
            if (const auto* ptr = std::get_if<std::shared_ptr<asset_type>>(&it->second))
                return *ptr;
            return nullptr;
        }

        registry& get_asset_registry() const { return _registry; }

    private:
        registry& _registry;
        std::unordered_map<std::string, asset_handle> _assets;
    };
}

