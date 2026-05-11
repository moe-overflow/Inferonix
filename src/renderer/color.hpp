#pragma once

namespace inferonix::renderer
{
    struct color
    {
        float r, g, b;
    };

    namespace colors
    {
        constexpr color black { .r=0.f, .g=0.f, .b=0.f };
        constexpr color white { .r=1.f, .g=1.f, .b=1.f };
        constexpr color green { .r=0.f, .g=1.f, .b=0.f };
        constexpr color red = { .r=1.f, .g=0.f, .b=1.f };
        constexpr color yellow = { .r=1.f, .g=1.f, .b=1.f };

    }

}


