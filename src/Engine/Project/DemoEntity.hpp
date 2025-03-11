#pragma once

#include "../EventSystem/KeyEvent.hpp"
#include "../Input/Input.hpp"
#include "../Renderer/RenderEntity.hpp"
#include "../Scene/Camera.hpp"

namespace Inferonix::Project
{

    class DemoEntity : public Renderer::RenderEntityData, public EventSystem::EventListener
    {
    public:
        DemoEntity()
        {
            Vertices = { -.5f, -.5f, .0f, .5f, -.5f, .0f, .0f, .5f, .0f };
            Indices = { 0, 1, 2 };
            DynamicallyColored = true;
        }

        void Update(float delta_time) override
        {
            using namespace Inferonix::InputSystem;


            float const player_speed = 1.F;
            float const rotate_speed = 30.F;

            if (Input::is_key_down(Key::UP))
                Transform.Translate(glm::vec3(0.0f, player_speed * delta_time, 0.0f));
            else if (Input::is_key_down(Key::LEFT))
                Transform.Translate(glm::vec3(-player_speed * delta_time, 0.0f, 0.0f));
            else if (Input::is_key_down(Key::DOWN))
                Transform.Translate(glm::vec3(0.0f, -player_speed * delta_time, 0.0f));
            else if (Input::is_key_down(Key::RIGHT))
                Transform.Translate(glm::vec3(player_speed * delta_time, 0.0f, 0.0f));

            Transform.Rotate(glm::vec3(0.0f, rotate_speed * delta_time, 0.0f));
        }


        void OnEvent(EventSystem::Event& event) override
        {
            using namespace InputSystem;

            if (auto type = dynamic_cast<EventSystem::KeyEvent*>(&event))
            {
                auto key = type->GetKey();
                if (type->GetType() == EventSystem::KeyEventType::KeyPressedEvent)
                    Input::set_key_down(key);
                else if (type->GetType() == EventSystem::KeyEventType::KeyReleasedEvent)
                    Input::set_key_up(key);
            }
        }
    };

} // namespace Inferonix::Project
