#pragma once

#include "../EventSystem/KeyEvent.hpp"
#include "../Input/Input.hpp"
#include "../Renderer/RenderEntity.hpp"

namespace Inferonix::Project
{

    class DemoEntity final : public Renderer::RenderEntityData
    {
    public:
        DemoEntity()
        {
            mesh_instance = std::make_shared<Renderer::Mesh>(RESOURCES_PATH "/models/Monkey.obj");
            dynamically_colored = true;
            transform.position = {2, 3, 1};
        }

        void Update(float const delta_time) override
        {
            using namespace Inferonix::InputSystem;


            constexpr float player_speed = 1.F;
            constexpr float rotate_speed = 30.F;

            if (Input::IsKeyDown(Key::UP))
                transform.Translate(glm::vec3(0.0f, player_speed * delta_time, 0.0f));
            else if (Input::IsKeyDown(Key::LEFT))
                transform.Translate(glm::vec3(-player_speed * delta_time, 0.0f, 0.0f));
            else if (Input::IsKeyDown(Key::DOWN))
                transform.Translate(glm::vec3(0.0f, -player_speed * delta_time, 0.0f));
            else if (Input::IsKeyDown(Key::RIGHT))
                transform.Translate(glm::vec3(player_speed * delta_time, 0.0f, 0.0f));

            transform.Rotate(glm::vec3(rotate_speed * delta_time, rotate_speed * delta_time, rotate_speed * delta_time));
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
