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

        static bool IsKeyDown(Key const& key);

        static void SetKeyDown(Key const& key);

        static void SetKeyUp(Key const& key);


    private:
        static std::unordered_map<Key, bool> _keys;
    };
} // namespace Inferonix::InputSystem
