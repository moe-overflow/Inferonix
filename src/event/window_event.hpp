#pragma once

#include "event.hpp"

namespace Inferonix::EventSystem
{
    enum class WindowEventType
    {
        WindowResize,
        WindowClose,

    };


    class window_event final : public event
    {
    public:
        window_event(WindowEventType const& type, int const x, int const y) : _type(type), _x(x), _y(y) { }

        explicit window_event(WindowEventType const type) : _type(type), _x(-1), _y(-1) { }

        [[nodiscard]] WindowEventType GetType() const
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

}
