#include "input.hpp"

#include <spdlog/spdlog.h>

using namespace inferonix::input;

std::unordered_map<Key, bool> input::_keys;
std::unordered_map<MouseKey, bool> input::_mouse_keys;

bool input::is_key_down(Key const& key)
{
    return _keys[key];
}


bool input::is_mouse_key_down(MouseKey const& key)
{
    return _mouse_keys[key];
}

void input::set_mouse_key_down(MouseKey const& key)
{
    _mouse_keys[key] = true;
}

void input::set_mouse_key_up(MouseKey const& key)
{
    _mouse_keys[key] = false;
}

void input::set_key_down(Key const& key)
{
    _keys[key] = true;
}

void input::set_key_up(Key const& key)
{
    _keys[key] = false;
}

float input::_mouse_x = 0.0f;
float input::_mouse_y = 0.0f;

std::pair<float, float> input::get_mouse_position()
{
    return { _mouse_x, _mouse_y };
}


void input::set_mouse_position(float x, float y)
{
    _mouse_x = x;
    _mouse_y = y;
}



