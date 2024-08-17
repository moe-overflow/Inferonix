#pragma once

#include "key_event.hpp"
#include "window_event.hpp"
#include "mouse_event.hpp"

#include <string>
#include <spdlog/spdlog.h>

namespace Inferonix::EventSystem
{
	class event_handler
	{
	public:
			
		static void handle_key_event(key_event& e)
		{
			switch (e.get_type())
			{
                case key_event_type::key_pressed_event:
                {
                    spdlog::info("Key pressed {}", e.get_key_code());
                    break;
                }
                case key_event_type::key_released_event:
                {
                    spdlog::info("Key released {}", e.get_key_code());
                    break;
                }
                case key_event_type::key_repeat_event:
                {
                    spdlog::info("Key repeated {}", e.get_key_code());
                    break;
                }
                default:
                    break;
                }
		}

		static void handle_mouse_event(mouse_event& e)
		{
			switch (e.get_type())
			{
				case mouse_event_type::mouse_pointer_moved:
				{
					auto *casted_e = dynamic_cast<mouse_cursor_moved*>(&e);
                    spdlog::info("MOUSE CURSOR MOVED TO: ({}, {})", casted_e->get_x(), casted_e->get_y());
					break;
			
				}
				case mouse_event_type::mouse_pointer_entered:
				{

					auto *casted_e = dynamic_cast<mouse_cursor_entered*>(&e);
					const std::string state(casted_e->is_within_window() ? "ENTERED" : "LEFT");

                    spdlog::info("MOUSE CURSOR HAS {} THE WINDOW", state);
					break;
				}
				case mouse_event_type::mouse_button_clicked:
				{
                    spdlog::info("MOUSE CURSOR CLICKED");
					break;
				}
				case mouse_event_type::mouse_button_released:
				{
                    spdlog::info("MOUSE CURSOR RELEASED");
					break;
				}				
				{
				default: 
					break;
				}
			}
		}

		static void handle_window_event(window_event& e)
		{
			switch (e.get_type())
			{
				case window_event_type::window_resize:
				{
                    spdlog::info("WINDOW RESIZE ({}, {})", e.get_x(), e.get_y());
					break;
				}
				case window_event_type::window_close:
				{
                    spdlog::info("WINDOW CLOSED");
					break;
				} 
				{
				default:
					break;
				}
			}
		}
	};

}