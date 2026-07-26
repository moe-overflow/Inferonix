#include <gtest/gtest.h>
#include <assets/asset_registry.hpp>
#include <entt/entt.hpp>

TEST(AssetRegistry, RegistersItself)
{
    auto registry = entt::registry{};
    auto assets = inferonix::asset::asset_registry {registry};
    auto* stored = registry.ctx().get<inferonix::asset::asset_registry*>();
    ASSERT_EQ(stored, &assets);
}

namespace
{
    struct test_asset
    {
        bool load_from_file(const std::filesystem::path&)
        {
            return true;
        }
    };

    struct counting_asset
    {
        static inline int load_count = 0;

        bool load_from_file(const std::filesystem::path&)
        {
            ++load_count;
            return true;
        }
    };

    struct failing_asset
    {
        bool load_from_file(const std::filesystem::path&)
        {
            return false;
        }
    };

}

TEST(AssetRegistry, LoadsNewAssetSuccessfully)
{
    auto registry = entt::registry{};
    auto assets = inferonix::asset::asset_registry {registry};

    auto result = assets.load<test_asset>(
        "player",
        "player.asset"
    );

    ASSERT_TRUE(result.has_value());
    ASSERT_NE(result.value(), nullptr);
}

TEST(AssetRegistry, ReturnsCachedAsset)
{
    auto registry = entt::registry{};
    auto assets = inferonix::asset::asset_registry {registry};
    auto const first = assets.load<test_asset>("player", "player.asset");
    auto const second = assets.load<test_asset>("player", "player.asset");

    ASSERT_TRUE(first.has_value());
    ASSERT_TRUE(second.has_value());

    ASSERT_EQ(first.value(), second.value());

}

TEST(AssetRegistry, DoesNotReloadCachedAsset)
{
    counting_asset::load_count = 0;

    auto registry = entt::registry{};
    auto assets = inferonix::asset::asset_registry {registry};

    auto const first = assets.load<counting_asset>("texture","texture.asset" );
    auto const second = assets.load<counting_asset>("texture", "texture.asset");

    ASSERT_TRUE(first.has_value());
    ASSERT_TRUE(second.has_value());
    ASSERT_EQ(counting_asset::load_count, 1);
}

TEST(AssetRegistry, FailsWhenAssetCannotBeLoaded)
{
    auto registry = entt::registry{};
    auto assets = inferonix::asset::asset_registry {registry};

    auto result = assets.load<failing_asset>(
        "broken_asset",
        "missing.file"
    );

    ASSERT_FALSE(result.has_value());
    ASSERT_NE(result.error().empty(), true);
}


TEST(AssetRegistry, CanRetrieveLoadedAsset)
{
    auto registry = entt::registry{};
    auto assets = inferonix::asset::asset_registry {registry};

    auto const loaded = assets.load<test_asset>(
        "player",
        "player.asset"
    );

    ASSERT_TRUE(loaded.has_value());
    ASSERT_EQ(assets.get<test_asset>("player"), loaded.value());
}


TEST(AssetRegistry, ReturnsNullForUnknownAsset)
{
    auto registry = entt::registry{};
    auto assets = inferonix::asset::asset_registry {registry};
    auto const result = assets.get<test_asset>("does_not_exist");
    ASSERT_EQ(result, nullptr);
}
