#pragma once

#include "KeyCodes.hpp"

#include <optional>
#include <unordered_map>

namespace Inferonix::InputSystem
{
    class Input
    {
    public:
        Input() = delete;

        Input(Input const&) = delete;

        Input& operator=(Input const&) = delete;

        static bool is_key_down(Key key);

        static void set_key_down(Key key);

        static void set_key_up(Key key);


        static std::optional<Key> glfw_to_key(int glfw_key_code);

    private:
        static std::unordered_map<Key, bool> keys;
    };
} // namespace Inferonix::InputSystem
