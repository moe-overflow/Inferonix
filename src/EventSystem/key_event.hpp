#pragma once

#include "event.hpp"

namespace Inferonix::EventSystem
{
	enum class key_event_type
	{
		none,
		key_pressed_event,
		key_repeat_event,
		key_released_event

	};


	class key_event : public event
	{
	public:
		key_event(const int key_code, key_event_type type)
			: _key_code(key_code), _type(type)
		{}

		[[nodiscard]] int get_key_code() const { return _key_code; }
		[[nodiscard]] key_event_type get_type() const { return _type; }

	private:
		int _key_code;
		key_event_type _type;
		
	};
	

}
