#include <gtest/gtest.h>
#include <math/transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

namespace
{
    bool vec_close(glm::vec3 const& a, glm::vec3 const& b, float eps = 1e-4f)
    {
        return glm::all(glm::epsilonEqual(a, b, eps));
    }

    bool mat_close(glm::mat4 const& a, glm::mat4 const& b, float eps = 1e-4f)
    {
        for (int col = 0; col < 4; ++col)
            for (int row = 0; row < 4; ++row)
                if (std::abs(a[col][row] - b[col][row]) > eps)
                    return false;
        return true;
    }
}

TEST(Transform, DefaultConstructsToIdentity)
{
    inferonix::math::transform t;

    EXPECT_EQ(t.position, glm::vec3(0.0f));
    EXPECT_EQ(t.rotation, glm::vec3(0.0f));
    EXPECT_EQ(t.scale, glm::vec3(1.0f));

    EXPECT_TRUE(mat_close(t.get_matrix(), glm::mat4(1.0f)));
}

TEST(Transform, TranslateOffsetsPosition)
{
    inferonix::math::transform t;
    t.translate(glm::vec3(1.0f, 2.0f, 3.0f));

    EXPECT_EQ(t.position, glm::vec3(1.0f, 2.0f, 3.0f));

    auto m = t.get_matrix();
    EXPECT_TRUE(vec_close(glm::vec3(m[3]), glm::vec3(1.0f, 2.0f, 3.0f)));
}

TEST(Transform, RotateAddsToRotation)
{
    inferonix::math::transform t;
    t.rotate(glm::vec3(10.0f, 20.0f, 30.0f));
    t.rotate(glm::vec3(5.0f, 5.0f, 5.0f));

    EXPECT_EQ(t.rotation, glm::vec3(15.0f, 25.0f, 35.0f));
}

TEST(Transform, ScaleByMultipliesScale)
{
    inferonix::math::transform t;
    t.scale_by(glm::vec3(2.0f, 2.0f, 2.0f));
    t.scale_by(glm::vec3(0.5f, 3.0f, 1.0f));

    EXPECT_EQ(t.scale, glm::vec3(1.0f, 6.0f, 2.0f));
}

TEST(Transform, NormalizeRotationWrapsIntoPositiveDegrees)
{
    inferonix::math::transform t;
    t.rotation = glm::vec3(450.0f, -90.0f, 0.0f);
    t.normalize_rotation();

    EXPECT_EQ(t.rotation, glm::vec3(90.0f, 270.0f, 0.0f));
}

TEST(Transform, GetMatrixAppliesPosition)
{
    inferonix::math::transform t;
    t.position = glm::vec3(10.0f, 0.0f, 0.0f);

    auto p = t.get_matrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    EXPECT_TRUE(vec_close(glm::vec3(p), glm::vec3(10.0f, 0.0f, 0.0f)));
}

TEST(Transform, GetMatrixAppliesScale)
{
    inferonix::math::transform t;
    t.scale = glm::vec3(2.0f, 3.0f, 4.0f);

    auto p = t.get_matrix() * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    EXPECT_TRUE(vec_close(glm::vec3(p), glm::vec3(2.0f, 3.0f, 4.0f)));
}

TEST(Transform, GetMatrixAppliesComposedTransform)
{
    inferonix::math::transform t;
    t.position = glm::vec3(1.0f, 0.0f, 0.0f);
    t.scale = glm::vec3(2.0f);

    auto p = t.get_matrix() * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    EXPECT_TRUE(vec_close(glm::vec3(p), glm::vec3(3.0f, 0.0f, 0.0f)));
}

TEST(Transform, SetFromMatrixRecoversPositionAndScale)
{
    inferonix::math::transform expected;
    expected.position = glm::vec3(1.0f, 2.0f, 3.0f);
    expected.rotation = glm::vec3(30.0f, 45.0f, 60.0f);
    expected.scale = glm::vec3(2.0f, 3.0f, 4.0f);

    inferonix::math::transform actual;
    actual.set_from_matrix(expected.get_matrix());

    EXPECT_TRUE(vec_close(actual.position, expected.position, 1e-3f));
    EXPECT_TRUE(vec_close(actual.scale, expected.scale, 1e-3f));
}

TEST(Transform, SetFromMatrixIdentityRoundTrips)
{
    inferonix::math::transform t;
    t.set_from_matrix(glm::mat4(1.0f));

    EXPECT_TRUE(vec_close(t.position, glm::vec3(0.0f)));
    EXPECT_TRUE(vec_close(t.scale, glm::vec3(1.0f)));
    EXPECT_EQ(t.rotation, glm::vec3(0.0f));
    EXPECT_TRUE(mat_close(t.get_matrix(), glm::mat4(1.0f)));
}
