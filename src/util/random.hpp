#pragma once

#include <random>

namespace inferonix::util
{
    template<typename T>
    concept random_number = std::is_floating_point_v<T> || std::is_integral_v<T>;

    template<random_number T>
    auto random(T min, T max) -> T
    {
        static auto generator = std::mt19937{ std::random_device{}() };
        if constexpr (std::integral<T>)
            return std::uniform_int_distribution { min, max }(generator);
        else
            return std::uniform_real_distribution { min, max }(generator);
    }
}

