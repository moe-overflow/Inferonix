#pragma once

#include <entt/entt.hpp>

#include <memory>
#include <any>
#include <expected>
#include <filesystem>
#include <spdlog/spdlog.h>

namespace Inferonix::Asset
{
    using Registry = entt::registry;

    template<typename T>
    concept LoadableAsset = requires(T t, const std::filesystem::path& path)
    {
        { t.LoadFromFile(path) } -> std::same_as<bool>;
    };

    class asset_registry
    {
    public:
        explicit asset_registry(Registry& registry);
        ~asset_registry() = default;

        asset_registry(const asset_registry&) = delete;
        asset_registry& operator=(const asset_registry&) = delete;

        template<typename AssetType>
        std::expected<std::shared_ptr<AssetType>, std::string>
        Load(std::string const& id, std::filesystem::path const& path)
        {
            spdlog::info("Attempting to load asset: {}", id);

            if (auto const it = _assets.find(id); it != _assets.end())
            {
                return std::any_cast<std::shared_ptr<AssetType>>(it->second);
            }

            auto asset = std::make_shared<AssetType>();
            if (!asset->LoadFromFile(path))
            {
                spdlog::error("Failed to load asset: {}", id);
                return std::unexpected(std::format("Failed to load asset {}", path.string()));
            }
            spdlog::info("Loaded asset from {} successfully!", path.string());

            _assets.emplace(id, asset);
            return asset;

        }

        template<LoadableAsset AssetType>
        std::shared_ptr<AssetType> Get(const std::string& id)
        {
            auto const it = _assets.find(id);
            if (it == _assets.end()) return nullptr;
            return std::any_cast<std::shared_ptr<AssetType>>(it->second);
        }

        Registry& GetAssetRegistry() const { return _registry; }

    private:
        Registry& _registry;
        std::unordered_map<std::string, std::any> _assets;
    };
}