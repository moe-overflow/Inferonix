#pragma once

#include "../Scene/camera.hpp"
#include "../Renderer/render_entity.hpp"
#include "Input/input.hpp"
#include "EventSystem/key_event.hpp"

namespace Inferonix::Project
{

    class demo_entity :
            public Inferonix::Renderer::render_entity_data,
            public Inferonix::EventSystem::event_listener
    {
    public:

        demo_entity()
        {
            vertices =
            {
                    -.5f, -.5f, .0f,
                    .5f, -.5f, .0f,
                    .0f, .5f, .0f
            };
            indices =
            {
                    0, 1, 2

            };
            dynamically_colored = true;
        }

        void update(float delta_time) override
        {
            using namespace Inferonix::Input;


            const float player_speed = 1.F;
            const float rotate_speed = 30.F;

            if (input::is_key_down(key::UP))
                transform.translate(glm::vec3(0.0f, player_speed * delta_time, 0.0f));
            else if (input::is_key_down(key::LEFT))
                transform.translate(glm::vec3(-player_speed * delta_time, 0.0f, 0.0f));
            else if (input::is_key_down(key::DOWN))
                transform.translate(glm::vec3(0.0f, -player_speed * delta_time, 0.0f));
            else if (input::is_key_down(key::RIGHT))
                transform.translate(glm::vec3(player_speed * delta_time, 0.0f, 0.0f));

            transform.rotate(glm::vec3(0.0f, rotate_speed * delta_time, 0.0f));

        }


        void on_event(EventSystem::event &event) override
        {
            using namespace Input;

            if (auto type = dynamic_cast<EventSystem::key_event *>(&event))
            {
                auto key = type->get_key();
                if (type->get_type() == EventSystem::key_event_type::key_pressed_event)
                    input::set_key_down(key);
                else if (type->get_type() == EventSystem::key_event_type::key_released_event)
                    input::set_key_up(key);

            }
        }


    };

}