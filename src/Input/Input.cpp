#include "Input.hpp"

#include <spdlog/spdlog.h>

using namespace Inferonix::InputSystem;

std::unordered_map<Key, bool> Input::_keys;
std::unordered_map<MouseKey, bool> Input::_mouse_keys;

bool Input::IsKeyDown(Key const& key)
{
    return _keys[key];
}


bool Input::IsMouseKeyDown(MouseKey const& key)
{
    return _mouse_keys[key];
}

void Input::SetMouseKeyDown(MouseKey const& key)
{
    _mouse_keys[key] = true;
}

void Input::SetMouseKeyUp(MouseKey const& key)
{
    _mouse_keys[key] = false;
}

void Input::SetKeyDown(Key const& key)
{
    _keys[key] = true;
}

void Input::SetKeyUp(Key const& key)
{
    _keys[key] = false;
}

float Input::_mouse_x = 0.0f;
float Input::_mouse_y = 0.0f;

std::pair<float, float> Input::GetMousePosition()
{
    return { _mouse_x, _mouse_y };
}


void Input::SetMousePosition(float x, float y)
{
    _mouse_x = x;
    _mouse_y = y;
}



