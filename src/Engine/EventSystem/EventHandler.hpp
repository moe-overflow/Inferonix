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

        EventHandler& operator=(EventHandler const&) = delete;

        EventHandler(EventHandler const&) = delete;

        inline static std::shared_ptr<EventHandler> Get()
        {
            static std::shared_ptr<EventHandler> event_system_instance{ new EventHandler };
            return event_system_instance;
        }

        inline void Dispatch(Event& event)
        {
            for (auto& event_listener : _event_listeners)
                event_listener->OnEvent(event);
        }

        inline void Subscribe(std::shared_ptr<EventListener> const& event_listener)
        {
            _event_listeners.push_back(event_listener);
        }

        inline void Unsubscribe(EventListener* event_listener)
        {
            // todo: implement me
        }

    private:
        std::vector<std::shared_ptr<EventListener>> _event_listeners;
    };
} // namespace Inferonix::EventSystem
