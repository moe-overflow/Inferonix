#include "Input.hpp"

#include <optional>

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

using namespace Inferonix::InputSystem;

std::unordered_map<Key, bool> Input::keys;

bool Input::is_key_down(Key key)
{
    return keys[key];
}

void Input::set_key_down(Key key)
{
    keys[key] = true;
}

void Input::set_key_up(Key key)
{
    keys[key] = false;
}

std::optional<Key> Input::glfw_to_key(int glfw_key_code)
{
    switch (glfw_key_code)
    {
        case GLFW_KEY_W:
            return Key::W;
        case GLFW_KEY_A:
            return Key::A;
        case GLFW_KEY_S:
            return Key::S;
        case GLFW_KEY_D:
            return Key::D;
        case GLFW_KEY_UP:
            return Key::UP;
        case GLFW_KEY_DOWN:
            return Key::DOWN;
        case GLFW_KEY_LEFT:
            return Key::LEFT;
        case GLFW_KEY_RIGHT:
            return Key::RIGHT;
        default:
            spdlog::warn("GLFW key code {} not known", glfw_key_code);
    }
    return std::nullopt;
}
