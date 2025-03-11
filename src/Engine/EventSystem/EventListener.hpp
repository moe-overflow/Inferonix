#pragma once

#include "Event.hpp"

namespace Inferonix::EventSystem
{

    // This class acts as an interface that each class subscribing an event should implement
    class EventListener
    {
    public:
        virtual ~EventListener() = default;

        virtual void OnEvent(Event& event) = 0;
    };

} // namespace Inferonix::EventSystem
