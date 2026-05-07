#pragma once

#include <memory>
#include <vector>

#include "event_listener.hpp"

namespace inferonix::events
{
    class event_handler
    {
    public:
        event_handler() = default;

        event_handler(event_handler const&) = delete;
        event_handler& operator=(event_handler const&) = delete;

        static std::shared_ptr<event_handler> get()
        {
            static std::shared_ptr<event_handler> event_system_instance{ new event_handler };
            return event_system_instance;
        }

        void dispatch(event& event) const
        {
            for (auto& event_listener : _event_listeners)
                event_listener->on_event(event);
        }

        void subscribe(std::shared_ptr<event_listener> const& event_listener)
        {
            _event_listeners.push_back(event_listener);
        }

        void unsubscribe(event_listener* event_listener)
        {
            // todo: implement me
        }

    private:
        std::vector<std::shared_ptr<event_listener>> _event_listeners;
    };
}
