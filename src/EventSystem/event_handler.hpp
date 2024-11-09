#pragma once

#include <memory>
#include <vector>

#include "event_listener.hpp"

namespace Inferonix::EventSystem
{
    class event_handler
    {
    public:

        event_handler() = default;

        event_handler &operator=(const event_handler&) = delete;

        event_handler(const event_handler&) = delete;

        inline static std::shared_ptr<event_handler> get()
        {
            static std::shared_ptr<event_handler> event_system_instance{new event_handler};
            return event_system_instance;
        }

        inline void dispatch(event &event)
        {
            for (auto &event_listener: _event_listeners)
            {
                event_listener->on_event(event);
            }
        }

        inline void subscribe(const std::shared_ptr<event_listener> &event_listener)
        {
            _event_listeners.push_back(event_listener);
        }

        inline void unsubscribe(event_listener *event_listener)
        {
            // todo: implement me
        }

    private:
        std::vector<std::shared_ptr<event_listener>> _event_listeners;


    };
}

