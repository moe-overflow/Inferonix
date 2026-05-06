#pragma once

#include "event.hpp"
#include "../input/key_codes.hpp"


namespace Inferonix::EventSystem
{
    enum class KeyEventType
    {
        None,
        KeyPressedEvent,
        KeyRepeatEvent,
        KeyReleasedEvent

    };


    class key_event final : public event
    {
    public:
        key_event(InputSystem::Key const& key, KeyEventType const type) : _key(key), _type(type) {}

        [[nodiscard]] InputSystem::Key GetKey() const
        {
            return _key;
        }

        [[nodiscard]] KeyEventType GetType() const
        {
            return _type;
        }

    private:
        InputSystem::Key _key;
        KeyEventType _type;
    };


} // namespace Inferonix::EventSystem
