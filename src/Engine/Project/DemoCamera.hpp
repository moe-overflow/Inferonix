#pragma once

#include "../EventSystem/EventListener.hpp"
#include "../EventSystem/KeyEvent.hpp"
#include "../Input/Input.hpp"
#include "../Scene/Camera.hpp"

namespace Inferonix::Project
{
    constexpr float camera_speed = 10.0f;

    class DemoCamera final : public Scene::Camera, public EventSystem::EventListener
    {

    public:
        DemoCamera() = default;

        void OnEvent(EventSystem::Event& event) override
        {
            using namespace Inferonix::InputSystem;

            if (auto const type = dynamic_cast<EventSystem::KeyEvent*>(&event))
            {
                auto const key = type->GetKey();
                if (type->GetType() == EventSystem::KeyEventType::KeyPressedEvent)
                {
                    Input::SetKeyDown(key);
                }
                else if (type->GetType() == EventSystem::KeyEventType::KeyReleasedEvent)
                {
                    Input::SetKeyUp(key);
                }
            }
        }

        void Update(float const delta) override
        {
            using namespace Inferonix::InputSystem;

            auto const position = GetSettings().position;
            auto const orientation = GetSettings().orientation;

            glm::vec3 const right = glm::normalize(glm::cross(orientation, GetSettings().up_vector));


            if (Input::IsKeyDown(Key::W))
                SetPosition(position + orientation * camera_speed * delta);
            if (Input::IsKeyDown(Key::S))
                SetPosition(position - orientation * camera_speed * delta);
            if (Input::IsKeyDown(Key::A))
                SetPosition(position - right * camera_speed * delta);
            if (Input::IsKeyDown(Key::D))
                SetPosition(position + right * camera_speed * delta);
        }
    };

} // namespace Inferonix::Project
