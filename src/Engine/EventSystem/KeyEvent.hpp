#pragma once

#include "Event.hpp"
#include "../Input/KeyCodes.hpp"


namespace Inferonix::EventSystem
{
    enum class KeyEventType
    {
        None,
        KeyPressedEvent,
        KeyRepeatEvent,
        KeyReleasedEvent

    };


    class KeyEvent : public Event
    {
    public:
        KeyEvent(InputSystem::Key key, KeyEventType type) : _key(key), _type(type) {}

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
