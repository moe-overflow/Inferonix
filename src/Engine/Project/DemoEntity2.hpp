#pragma once

#include "../EventSystem/KeyEvent.hpp"
#include "../Input/Input.hpp"
#include "../Renderer/RenderEntity.hpp"
#include "../Scene/Camera.hpp"

namespace Inferonix::Project
{

    class DemoEntity2 final : public  Renderer::RenderEntityData
    {
    public:
        DemoEntity2()
        {
            mesh_instance = std::make_shared<Renderer::Mesh>(RESOURCES_PATH "/models/Cylinder.obj");
            dynamically_colored = true;
        }

        void Update(float delta_time) override
        {
            using namespace InputSystem;

            constexpr float player_speed = 1.F;
            constexpr float rotate_speed = 30.F;

            if (Input::IsKeyDown(Key::I))
                transform.Translate(glm::vec3(0.0f, player_speed * delta_time, 0.0f));
            else if (Input::IsKeyDown(Key::J))
                transform.Translate(glm::vec3(-player_speed * delta_time, 0.0f, 0.0f));
            else if (Input::IsKeyDown(Key::K))
                transform.Translate(glm::vec3(0.0f, -player_speed * delta_time, 0.0f));
            else if (Input::IsKeyDown(Key::L))
                transform.Translate(glm::vec3(player_speed * delta_time, 0.0f, 0.0f));

            transform.Rotate(glm::vec3(0.F, rotate_speed * delta_time, 0.F));
        }


        void OnEvent(EventSystem::Event& event) override
        {
            using namespace InputSystem;

            if (auto const type = dynamic_cast<EventSystem::KeyEvent*>(&event))
            {
                auto const key = type->GetKey();
                if (type->GetType() == EventSystem::KeyEventType::KeyPressedEvent)
                    Input::SetKeyDown(key);
                else if (type->GetType() == EventSystem::KeyEventType::KeyReleasedEvent)
                    Input::SetKeyUp(key);
            }
        }
    };

} // namespace Inferonix::Project
