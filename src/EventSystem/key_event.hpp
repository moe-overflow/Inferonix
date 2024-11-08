#pragma once

#include "event.hpp"

#include "../Input/input.hpp"
#include "../Input/key_codes.hpp"

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
		key_event(Input::key key, key_event_type type)
			: _key(key), _type(type)
		{}

		[[nodiscard]] Input::key get_key() const { return _key; }
		[[nodiscard]] key_event_type get_type() const { return _type; }

	private:
		Input::key _key;
		key_event_type _type;
		
	};
	

}
