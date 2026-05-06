#pragma once

#include "key_codes.hpp"
#include "mouse_codes.hpp"

#include <optional>
#include <unordered_map>

namespace Inferonix::InputSystem
{
    class input
    {
    public:
        input() = delete;
        input(input const&) = delete;

        input& operator=(input const&) = delete;

        static bool IsKeyDown(Key const& key);
        static void SetKeyDown(Key const& key);
        static void SetKeyUp(Key const& key);

        static bool IsMouseKeyDown(MouseKey const& key);
        static void SetMouseKeyDown(MouseKey const& key);
        static void SetMouseKeyUp(MouseKey const& key);


        static std::pair<float, float> GetMousePosition();
        static void SetMousePosition(float x, float y);


    private:
        static std::unordered_map<Key, bool> _keys;
        static std::unordered_map<MouseKey, bool> _mouse_keys;


        static float _mouse_x, _mouse_y;

    };
}
