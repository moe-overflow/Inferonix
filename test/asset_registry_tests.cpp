#include <gtest/gtest.h>
#include <assets/asset_registry.hpp>
#include <scene/model.hpp>
#include <entt/entt.hpp>

#include <filesystem>
#include <string>

namespace
{
    auto asset_path(const char* relative) -> std::filesystem::path
    {
        return std::filesystem::path{TEST_ASSETS_DIR} / relative;
    }
}

TEST(AssetRegistry, RegistersItself)
{
    auto registry = entt::registry{};
    auto assets = inferonix::asset::asset_registry {registry};
    auto* stored = registry.ctx().get<inferonix::asset::asset_registry*>();
    ASSERT_EQ(stored, &assets);
}

TEST(AssetRegistry, LoadsModelSuccessfully)
{
    auto registry = entt::registry{};
    auto assets = inferonix::asset::asset_registry {registry};
    const auto result = assets.load<inferonix::scene::model>("triangle", asset_path("models/Triangle.obj"));

    ASSERT_TRUE(result.has_value());
    ASSERT_NE(result.value(), nullptr);
}

TEST(AssetRegistry, ReturnsCachedModel)
{
    auto registry = entt::registry{};
    auto assets = inferonix::asset::asset_registry {registry};

    auto const first = assets.load<inferonix::scene::model>("triangle", asset_path("models/Triangle.obj"));
    auto const second = assets.load<inferonix::scene::model>("triangle", asset_path("models/Triangle.obj"));

    ASSERT_TRUE(first.has_value());
    ASSERT_TRUE(second.has_value());
    ASSERT_EQ(first.value(), second.value());
}

TEST(AssetRegistry, LoadsDistinctIdsIndependently)
{
    auto registry = entt::registry{};
    auto assets = inferonix::asset::asset_registry {registry};

    auto const a = assets.load<inferonix::scene::model>("triangle", asset_path("models/Triangle.obj"));
    auto const b = assets.load<inferonix::scene::model>("monkey", asset_path("models/Monkey.obj"));

    ASSERT_TRUE(a.has_value());
    ASSERT_TRUE(b.has_value());
    ASSERT_NE(a.value(), b.value());
}

TEST(AssetRegistry, FailsWhenModelCannotBeLoaded)
{
    auto registry = entt::registry{};
    auto assets = inferonix::asset::asset_registry {registry};

    auto result = assets.load<inferonix::scene::model>("missing", asset_path("models/does_not_exist.obj"));

    ASSERT_FALSE(result.has_value());
    ASSERT_FALSE(result.error().empty());
}

TEST(AssetRegistry, FailsWhenTextureCannotBeLoaded)
{
    auto registry = entt::registry{};
    auto assets = inferonix::asset::asset_registry {registry};
    auto result = assets.load<inferonix::renderer::texture>("missing_tex", asset_path("textures/does_not_exist.png"));

    ASSERT_FALSE(result.has_value());
    ASSERT_FALSE(result.error().empty());
}

TEST(AssetRegistry, CanRetrieveLoadedAsset)
{
    auto registry = entt::registry{};
    auto assets = inferonix::asset::asset_registry {registry};

    auto const loaded = assets.load<inferonix::scene::model>("triangle", asset_path("models/Triangle.obj"));

    ASSERT_TRUE(loaded.has_value());
    ASSERT_EQ(assets.get<inferonix::scene::model>("triangle"), loaded.value());
}

TEST(AssetRegistry, ReturnsNullForUnknownAsset)
{
    auto registry = entt::registry{};
    auto assets = inferonix::asset::asset_registry {registry};
    auto const result = assets.get<inferonix::scene::model>("does_not_exist");
    ASSERT_EQ(result, nullptr);
}
