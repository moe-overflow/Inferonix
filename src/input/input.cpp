#include "input.hpp"

#include <spdlog/spdlog.h>

using namespace Inferonix::InputSystem;

std::unordered_map<Key, bool> input::_keys;
std::unordered_map<MouseKey, bool> input::_mouse_keys;

bool input::IsKeyDown(Key const& key)
{
    return _keys[key];
}


bool input::IsMouseKeyDown(MouseKey const& key)
{
    return _mouse_keys[key];
}

void input::SetMouseKeyDown(MouseKey const& key)
{
    _mouse_keys[key] = true;
}

void input::SetMouseKeyUp(MouseKey const& key)
{
    _mouse_keys[key] = false;
}

void input::SetKeyDown(Key const& key)
{
    _keys[key] = true;
}

void input::SetKeyUp(Key const& key)
{
    _keys[key] = false;
}

float input::_mouse_x = 0.0f;
float input::_mouse_y = 0.0f;

std::pair<float, float> input::GetMousePosition()
{
    return { _mouse_x, _mouse_y };
}


void input::SetMousePosition(float x, float y)
{
    _mouse_x = x;
    _mouse_y = y;
}



