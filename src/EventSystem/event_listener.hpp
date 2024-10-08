#pragma once

#include "event.hpp"

namespace Inferonix::EventSystem
{

    // This class acts as an interface that each class subscribing an event should implement
    class event_listener
    {
    public:
        virtual ~event_listener() = default;
        virtual void on_event(event& event) = 0;

    };

}