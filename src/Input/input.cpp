#include "input.hpp"

#include <optional>
#include <spdlog/spdlog.h>

using namespace Inferonix::Input;

std::unordered_map<key, bool> input::keys;

bool input::is_key_down(key key)
{
    return keys[key];
}

void input::set_key_down(key key)
{
    keys[key] = true;
}

void input::set_key_up(key key)
{
    keys[key] = false;
}

std::optional<key> input::glfw_to_key(int glfw_key_code)
{
    switch (glfw_key_code)
    {
        case GLFW_KEY_W: return key::W;
        case GLFW_KEY_A: return key::A;
        case GLFW_KEY_S: return key::S;
        case GLFW_KEY_D: return key::D;
        case GLFW_KEY_UP: return key::UP;
        case GLFW_KEY_DOWN: return key::DOWN;
        case GLFW_KEY_LEFT: return key::LEFT;
        case GLFW_KEY_RIGHT: return key::RIGHT;
        default: spdlog::warn("GLFW key code {} not known", glfw_key_code);
    }
    return std::nullopt;
}

