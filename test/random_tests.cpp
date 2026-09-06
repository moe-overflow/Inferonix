#include <gtest/gtest.h>
#include <util/random.hpp>

namespace
{
    template<typename T>
    void assert_inside(T value, T min, T max)
    {
        EXPECT_GE(value, min);
        EXPECT_LE(value, max);
    }
}

TEST(Random, IntegerWithinInclusiveBounds)
{
    for (int i = 0; i < 1000; ++i)
        assert_inside(inferonix::util::random(1, 6), 1, 6);
}

TEST(Random, IntegerProducesVariedResults)
{
    int unique = 0;
    int last = inferonix::util::random(0, 1000000);
    for (int i = 0; i < 100; ++i)
    {
        int const next = inferonix::util::random(0, 1000000);
        if (next != last)
            ++unique;
        last = next;
    }
    EXPECT_GT(unique, 0);
}

TEST(Random, IntegerSingleValueRange)
{
    assert_inside(inferonix::util::random(7, 7), 7, 7);
}

TEST(Random, IntegerNegativeRange)
{
    for (int i = 0; i < 1000; ++i)
        assert_inside(inferonix::util::random(-10, -1), -10, -1);
}

TEST(Random, FloatWithinBounds)
{
    for (int i = 0; i < 1000; ++i)
        assert_inside(inferonix::util::random(0.0f, 1.0f), 0.0f, 1.0f);
}

TEST(Random, FloatNegativeBounds)
{
    for (int i = 0; i < 1000; ++i)
        assert_inside(inferonix::util::random(-2.5f, -0.5f), -2.5f, -0.5f);
}

TEST(Random, DoubleWithinBounds)
{
    for (int i = 0; i < 1000; ++i)
        assert_inside(inferonix::util::random(10.0, 20.0), 10.0, 20.0);
}
