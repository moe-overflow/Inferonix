#pragma once

#include "../Scene/camera.hpp"
#include "../EventSystem/event_listener.hpp"
#include "../EventSystem/key_event.hpp"
#include "../Input/input.hpp"

namespace Inferonix::Project
{
    const float camera_speed = 10.0f;

    class demo_camera :
            public Inferonix::Scene::camera,
            public Inferonix::EventSystem::event_listener
    {

    public:

        demo_camera() = default;

        void on_event(Inferonix::EventSystem::event& event) override
        {
            using namespace Inferonix::Input;

            if(auto type = dynamic_cast<Inferonix::EventSystem::key_event*>(&event))
            {
                auto key = type->get_key();
                if(type->get_type() == Inferonix::EventSystem::key_event_type::key_pressed_event)
                {
                    input::set_key_down(key);
                }
                else if(type->get_type() == Inferonix::EventSystem::key_event_type::key_released_event)
                {
                    input::set_key_up(key);
                }

            }

        }

        void update(float delta) override
        {
            using namespace Inferonix::Input;

            auto position = get_settings()._position;
            auto orientation = get_settings()._orientation;

            glm::vec3 right = glm::normalize(glm::cross(orientation, get_settings()._up_vector));


            if(input::is_key_down(key::W))    set_position(position + orientation * camera_speed * delta);
            if(input::is_key_down(key::S))  set_position(position - orientation * camera_speed * delta);
            if(input::is_key_down(key::A))  set_position(position - right * camera_speed * delta);
            if(input::is_key_down(key::D)) set_position(position + right * camera_speed * delta);

        }
    };

}