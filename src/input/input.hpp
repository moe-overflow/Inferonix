#pragma once

#include "key_codes.hpp"
#include "mouse_codes.hpp"

#include <optional>
#include <unordered_map>

namespace inferonix::input
{
    class input
    {
    public:
        input() = delete;
        input(input const&) = delete;

        input& operator=(input const&) = delete;

        static bool is_key_down(Key const& key);
        static void set_key_down(Key const& key);
        static void set_key_up(Key const& key);

        static bool is_mouse_key_down(MouseKey const& key);
        static void set_mouse_key_down(MouseKey const& key);
        static void set_mouse_key_up(MouseKey const& key);


        static std::pair<float, float> get_mouse_position();
        static void set_mouse_position(float x, float y);


    private:
        static std::unordered_map<Key, bool> _keys;
        static std::unordered_map<MouseKey, bool> _mouse_keys;


        static float _mouse_x, _mouse_y;

    };
}
