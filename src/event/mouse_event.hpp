#pragma once

#include "event.hpp"

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


    class mouse_event : public event
    {
    public:
        mouse_event() : _type(MouseEventType::None) { }

        explicit mouse_event(MouseEventType type) : _type(type) { }

        [[nodiscard]] MouseEventType GetType() const
        {
            return _type;
        }


    public:
        MouseEventType _type;
    };

    class MouseButton final : public mouse_event
    {
    public:
        MouseButton(MouseEventType const& type, int const button) : mouse_event(type), _button_code(button) { }

        MouseButton() : mouse_event(MouseEventType::None), _button_code(-1) { }

    private:
        int _button_code;
    };


    class MouseCursorEntered final : public mouse_event
    {
    public:
        MouseCursorEntered(MouseEventType const& type, bool const in) : mouse_event(type), _cursor_within_window(in) { }

        [[nodiscard]] bool IsWithinWindow() const
        {
            return _cursor_within_window;
        }

    private:
        bool _cursor_within_window;
    };

    class MouseCursorMoved final : public mouse_event
    {
    public:
        MouseCursorMoved(MouseEventType const& type, int const x, int const y) : mouse_event(type), _x(x), _y(y) { }

    private:
        int _x, _y;
    };


}
