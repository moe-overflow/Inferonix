#pragma once

#include "../EventSystem/KeyEvent.hpp"
#include "../Input/Input.hpp"
#include "../Renderer/RenderEntity.hpp"
#include "../Scene/Camera.hpp"

namespace Inferonix::Project
{

    class DemoEntity2 : public  Renderer::RenderEntityData
    {
    public:
        DemoEntity2()
        {
            MeshInstance = std::make_shared<Renderer::Mesh>(RESOURCES_PATH "/models/Cylinder.obj");
            DynamicallyColored = true;
        }

        void Update(float delta_time) override
        {
            using namespace Inferonix::InputSystem;


            float const player_speed = 1.F;
            float const rotate_speed = 30.F;

            if (Input::IsKeyDown(Key::I))
                Transform.Translate(glm::vec3(0.0f, player_speed * delta_time, 0.0f));
            else if (Input::IsKeyDown(Key::J))
                Transform.Translate(glm::vec3(-player_speed * delta_time, 0.0f, 0.0f));
            else if (Input::IsKeyDown(Key::K))
                Transform.Translate(glm::vec3(0.0f, -player_speed * delta_time, 0.0f));
            else if (Input::IsKeyDown(Key::L))
                Transform.Translate(glm::vec3(player_speed * delta_time, 0.0f, 0.0f));

            Transform.Rotate(glm::vec3(0.F, rotate_speed * delta_time, 0.F));
        }


        void OnEvent(EventSystem::Event& event) override
        {
            using namespace InputSystem;

            if (auto type = dynamic_cast<EventSystem::KeyEvent*>(&event))
            {
                auto key = type->GetKey();
                if (type->GetType() == EventSystem::KeyEventType::KeyPressedEvent)
                    Input::SetKeyDown(key);
                else if (type->GetType() == EventSystem::KeyEventType::KeyReleasedEvent)
                    Input::SetKeyUp(key);
            }
        }
    };

} // namespace Inferonix::Project
