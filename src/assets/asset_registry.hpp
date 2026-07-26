#pragma once

#include <entt/entt.hpp>

#include <memory>
#include <any>
#include <expected>
#include <filesystem>
#include <util/logger.hpp>

namespace inferonix::asset
{
    using registry = entt::registry;

    template<typename T>
    concept loadable_asset = requires(T t, const std::filesystem::path& path)
    {
        { t.load_from_file(path) } -> std::same_as<bool>;
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

        template<typename asset_type>
        std::expected<std::shared_ptr<asset_type>, std::string>
        load(std::string const& id, std::filesystem::path const& path)
        {
            LOG(LOG_TYPE::INFO, "Attempting to load asset: {}", id);

            if (auto const it = _assets.find(id); it != _assets.end())
            {
                return std::any_cast<std::shared_ptr<asset_type>>(it->second);
            }

            auto asset = std::make_shared<asset_type>();
            if (!asset->load_from_file(path))
            {
                spdlog::error("Failed to load asset: {}", id);
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
            if (it == _assets.end()) return nullptr;
            return std::any_cast<std::shared_ptr<asset_type>>(it->second);
        }

        registry& get_asset_registry() const { return _registry; }

    private:
        registry& _registry;
        std::unordered_map<std::string, std::any> _assets;
    };
}

