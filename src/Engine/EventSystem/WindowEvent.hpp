#pragma once

#include "Event.hpp"

namespace Inferonix::EventSystem
{
    enum class WindowEventType
    {
        WindowResize,
        WindowClose,

    };


    class WindowEvent : public Event
    {
    public:
        WindowEvent(WindowEventType type, int x, int y) : _type(type), _x(x), _y(y) { }

        explicit WindowEvent(WindowEventType type) : _type(type), _x(-1), _y(-1) { }

        [[nodiscard]] WindowEventType GetType()
        {
            return _type;
        }

        [[nodiscard]] int get_x() const
        {
            return _x;
        }

        [[nodiscard]] int get_y() const
        {
            return _y;
        }


    private:
        WindowEventType _type;
        int _x, _y; // todo: use vec2
    };

} // namespace Inferonix::EventSystem
