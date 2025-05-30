#pragma once

#include "Event.hpp"

namespace Inferonix::EventSystem
{
    enum class MouseEventType
    {
        None = 0,
        MouseButtonClicked,
        MouseButtonReleased,
        MousePointerMoved,
        MousePointerEntered

    };


    class MouseEvent : public Event
    {
    public:
        MouseEvent() : _type(MouseEventType::None) { }

        explicit MouseEvent(MouseEventType type) : _type(type) { }

        [[nodiscard]] MouseEventType GetType() const
        {
            return _type;
        }


    public:
        MouseEventType _type;
    };

    class MouseButton final : public MouseEvent
    {
    public:
        MouseButton(MouseEventType const& type, int const button) : MouseEvent(type), _button_code(button) { }

        MouseButton() : MouseEvent(MouseEventType::None), _button_code(-1) { }

    private:
        int _button_code;
    };


    class MouseCursorEntered final : public MouseEvent
    {
    public:
        MouseCursorEntered(MouseEventType const& type, bool const in) : MouseEvent(type), _cursor_within_window(in) { }

        [[nodiscard]] bool IsWithinWindow() const
        {
            return _cursor_within_window;
        }

    private:
        bool _cursor_within_window;
    };

    class MouseCursorMoved final : public MouseEvent
    {
    public:
        MouseCursorMoved(MouseEventType const& type, int const x, int const y) : MouseEvent(type), _x(x), _y(y) { }

    private:
        int _x, _y;
    };


} // namespace Inferonix::EventSystem
