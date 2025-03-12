#pragma once

#include "../EventSystem/EventListener.hpp"
#include "../EventSystem/KeyEvent.hpp"
#include "../Input/Input.hpp"
#include "../Scene/Camera.hpp"

namespace Inferonix::Project
{
    float const camera_speed = 10.0f;

    class DemoCamera : public Inferonix::Scene::Camera, public Inferonix::EventSystem::EventListener
    {

    public:
        DemoCamera() = default;

        void OnEvent(EventSystem::Event& event) override
        {
            using namespace Inferonix::InputSystem;

            if (auto type = dynamic_cast<Inferonix::EventSystem::KeyEvent*>(&event))
            {
                auto key = type->GetKey();
                if (type->GetType() == Inferonix::EventSystem::KeyEventType::KeyPressedEvent)
                {
                    Input::SetKeyDown(key);
                }
                else if (type->GetType() == Inferonix::EventSystem::KeyEventType::KeyReleasedEvent)
                {
                    Input::SetKeyUp(key);
                }
            }
        }

        void Update(float delta) override
        {
            using namespace Inferonix::InputSystem;

            auto position = GetSettings().Position;
            auto orientation = GetSettings().Orientation;

            glm::vec3 right = glm::normalize(glm::cross(orientation, GetSettings().UpVector));


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
