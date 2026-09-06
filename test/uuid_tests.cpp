#include <gtest/gtest.h>
#include <util/uuid.hpp>

#include <set>
#include <string>
#include <glad/glad.h>

namespace
{
    bool is_hex_digit(char c)
    {
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
    }

    bool is_valid_uuid(std::string const& uuid)
    {
        if (uuid.size() != 36)
            return false;

        int const dash_positions[] = { 8, 13, 18, 23 };
        int dash_index = 0;
        for (int i = 0; i < 36; ++i)
        {
            if (dash_index < 4 && i == dash_positions[dash_index])
            {
                if (uuid[i] != '-')
                    return false;
                ++dash_index;
                continue;
            }
            if (!is_hex_digit(uuid[i]))
                return false;
        }
        return dash_index == 4;
    }
}

TEST(Uuid, IsStdFormat)
{
    EXPECT_TRUE(is_valid_uuid(inferonix::utils::generate_uuid()));
}

TEST(Uuid, ProducesDistinctValues)
{
    auto const first = inferonix::utils::generate_uuid();
    auto const second = inferonix::utils::generate_uuid();
    EXPECT_NE(first, second);
}

TEST(Uuid, GeneratesManyUniqueValues)
{
    std::set<std::string> seen;
    for (int i = 0; i < 1000; ++i)
        seen.insert(inferonix::utils::generate_uuid());

    EXPECT_EQ(seen.size(), 1000u);
}
