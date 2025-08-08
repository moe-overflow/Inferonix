#pragma once

#include <memory>
#include <vector>

#include "EventListener.hpp"

namespace Inferonix::EventSystem
{
    class EventHandler
    {
    public:
        EventHandler() = default;

        EventHandler(EventHandler const&) = delete;
        EventHandler& operator=(EventHandler const&) = delete;

        static std::shared_ptr<EventHandler> Get()
        {
            static std::shared_ptr<EventHandler> event_system_instance{ new EventHandler };
            return event_system_instance;
        }

        void Dispatch(Event& event) const
        {
            for (auto& event_listener : _event_listeners)
                event_listener->OnEvent(event);
        }

        void Subscribe(std::shared_ptr<EventListener> const& event_listener)
        {
            _event_listeners.push_back(event_listener);
        }

        void Unsubscribe(EventListener* event_listener)
        {
            // todo: implement me
        }

    private:
        std::vector<std::shared_ptr<EventListener>> _event_listeners;
    };
}
