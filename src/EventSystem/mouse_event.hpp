#pragma once

#include "event.hpp"

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


	class mouse_event : event 
	{
	public:

        mouse_event() : _type(mouse_event_type::none) {}
		explicit mouse_event(mouse_event_type type) : _type(type) {}

		[[nodiscard]] mouse_event_type get_type() const { return _type; }


	public:
		mouse_event_type _type;
	};

	class mouse_button : public mouse_event
	{
	public: 
		mouse_button(mouse_event_type type, int button)
			: mouse_event(type), _button_code(button)
		{}

		mouse_button() : mouse_event(mouse_event_type::none), _button_code(-1)
		{}

	private:
		int _button_code;
	};

							

	class mouse_cursor_entered : public mouse_event
	{
	public:

		mouse_cursor_entered(mouse_event_type type, bool in) : mouse_event(type), _cursor_within_window(in)
		{}

		[[nodiscard]] bool is_within_window() const { return _cursor_within_window; }

	private:
		bool _cursor_within_window;

	};

	class mouse_cursor_moved : public mouse_event
	{
	public:
		mouse_cursor_moved(mouse_event_type type, int x, int y)
			: mouse_event(type), _x(x), _y(y)
		{}

		[[nodiscard]] int get_x() const { return _x; }
        [[nodiscard]] int get_y() const { return _y; }

	private:
		int _x, _y;
	};


}
