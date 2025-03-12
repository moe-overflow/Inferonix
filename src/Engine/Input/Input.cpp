#include "Input.hpp"

#include <spdlog/spdlog.h>

using namespace Inferonix::InputSystem;

std::unordered_map<Key, bool> Input::_keys;

bool Input::IsKeyDown(Key key)
{
    return _keys[key];
}

void Input::SetKeyDown(Key key)
{
    _keys[key] = true;
}

void Input::SetKeyUp(Key key)
{
    _keys[key] = false;
}


