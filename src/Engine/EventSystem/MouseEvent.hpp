#pragma once

#include "Event.hpp"

namespace Inferonix::EventSystem
{
    enum class mouse_event_type
    {
        none = 0,
        mouse_button_clicked,
        mouse_button_released,
        mouse_pointer_moved,
        mouse_pointer_entered

    };


    class MouseEvent : public Event
    {
    public:
        MouseEvent() : _type(mouse_event_type::none) { }

        explicit MouseEvent(mouse_event_type type) : _type(type) { }

        [[nodiscard]] mouse_event_type GetType() const
        {
            return _type;
        }


    public:
        mouse_event_type _type;
    };

    class MouseButton : public MouseEvent
    {
    public:
        MouseButton(mouse_event_type type, int button) : MouseEvent(type), _button_code(button) { }

        MouseButton() : MouseEvent(mouse_event_type::none), _button_code(-1) { }

    private:
        int _button_code;
    };


    class MouseCursorEntered : public MouseEvent
    {
    public:
        MouseCursorEntered(mouse_event_type type, bool in) : MouseEvent(type), _cursor_within_window(in) { }

        [[nodiscard]] bool IsWithinWindow() const
        {
            return _cursor_within_window;
        }

    private:
        bool _cursor_within_window;
    };

    class MouseCursorMoved : public MouseEvent
    {
    public:
        MouseCursorMoved(mouse_event_type type, int x, int y) : MouseEvent(type), _x(x), _y(y) { }

    private:
        int _x, _y;
    };


} // namespace Inferonix::EventSystem
