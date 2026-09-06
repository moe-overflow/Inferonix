#include <gtest/gtest.h>
#include <scene/camera.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

TEST(Camera, DefaultSettingsAreReasonable)
{
    auto camera = inferonix::scene::camera{};
    auto settings = camera.get_settings();

    EXPECT_FLOAT_EQ(settings.fov, 45.0f);
    EXPECT_FLOAT_EQ(settings.aspect_ratio, 16.0f / 9.0f);
    EXPECT_FLOAT_EQ(settings.near_plane, 0.1f);
    EXPECT_FLOAT_EQ(settings.far_plane, 100.0f);
    EXPECT_FALSE(settings.is_primary);
}

TEST(Camera, IsPrimaryReflectsSettings)
{
    inferonix::scene::camera_settings settings;
    settings.is_primary = true;

    inferonix::scene::camera cam(settings);
    EXPECT_TRUE(cam.is_primary());
}

TEST(Camera, SetPositionUpdatesView)
{
    inferonix::scene::camera cam;
    auto const before = cam.get_view();

    cam.set_position(glm::vec3(1.0f, 2.0f, 3.0f));

    EXPECT_NE(cam.get_view(), before);
    EXPECT_EQ(cam.get_settings().position, glm::vec3(1.0f, 2.0f, 3.0f));
}

TEST(Camera, SetOrientationUpdatesView)
{
    inferonix::scene::camera cam;
    auto const before = cam.get_view();

    cam.set_orientation(glm::vec3(0.0f, 1.0f, 0.0f));

    EXPECT_NE(cam.get_view(), before);
    EXPECT_EQ(cam.get_settings().orientation, glm::vec3(0.0f, 1.0f, 0.0f));
}

TEST(Camera, DefaultViewMapsCameraPositionToOrigin)
{
    const auto camera = inferonix::scene::camera{};

    auto const position = camera.get_settings().position;
    auto const view = camera.get_view();

    auto const mapped = view * glm::vec4(position, 1.0f);
    EXPECT_TRUE(glm::all(glm::epsilonEqual(glm::vec3(mapped), glm::vec3(0.0f, 0.0f, 0.0f), 1e-4f)));
}

TEST(Camera, ProjectionClipsToNearAndFar)
{
    auto settings = inferonix::scene::camera_settings{};
    settings.near_plane = 0.1f;
    settings.far_plane = 100.0f;
    auto camera = inferonix::scene::camera {settings};

    const glm::vec4 far = camera.get_projection() * glm::vec4(0.0f, 0.0f, -100.0f, 1.0f);

    float const far_ndc_z = far.z / far.w;
    EXPECT_FLOAT_EQ(far_ndc_z, 1.0f);
}

TEST(Camera, ProjectionReflectsAspectRatio)
{
    auto wide = inferonix::scene::camera_settings{}, square = inferonix::scene::camera_settings{};
    wide.aspect_ratio = 2.0f;
    square.aspect_ratio = 1.0f;

    auto camera_wide = inferonix::scene::camera{wide}, camera_square = inferonix::scene::camera{square};

    glm::vec4 const point { 1.0f, 0.0f, -5.0f, 1.0f };
    glm::vec4 const clipped_wide   = camera_wide.get_projection()   * point;
    glm::vec4 const clipped_square = camera_square.get_projection() * point;

    EXPECT_LT(clipped_wide.x / clipped_wide.w, clipped_square.x / clipped_square.w);
}
