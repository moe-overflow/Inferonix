#pragma once

#include "Camera.hpp"
#include "../Input/Input.hpp"
#include "../Input/MouseCodes.hpp"

namespace Inferonix::Scene
{
    class EditorCamera final : public Camera
    {
    public:
        explicit EditorCamera(CameraSettings const& settings) : Camera(settings)
        {
            UpdateCameraVectors();
        }

        void Update(float delta_time) override
        {
            if (InputSystem::Input::IsMouseKeyDown(InputSystem::MouseKey::RIGHT_BUTTON))
            {
                auto [mouse_x, mouse_y] = InputSystem::Input::GetMousePosition();
                if (_first_mouse) {
                    _last_x = mouse_x;
                    _last_y = mouse_y;
                    _first_mouse = false;
                }

                float x_offset = mouse_x - _last_x;
                float y_offset = _last_y - mouse_y; // reversed
                _last_x = mouse_x;
                _last_y = mouse_y;

                _yaw += x_offset * _mouse_sensitivity;
                _pitch += y_offset * _mouse_sensitivity;

                if (_pitch > 89.0f) _pitch = 89.0f;
                if (_pitch < -89.0f) _pitch = -89.0f;

                UpdateCameraVectors();
            }
            else
            {
                _first_mouse = true;
            }

            float velocity = _movement_speed * delta_time;
            glm::vec3 pos = GetSettings().position;

            if (InputSystem::Input::IsKeyDown(InputSystem::Key::W)) pos += _front * velocity;
            if (InputSystem::Input::IsKeyDown(InputSystem::Key::S)) pos -= _front * velocity;
            if (InputSystem::Input::IsKeyDown(InputSystem::Key::A)) pos -= _right * velocity;
            if (InputSystem::Input::IsKeyDown(InputSystem::Key::D)) pos += _right * velocity;

            // Up/Down (E and Q)
            if (InputSystem::Input::IsKeyDown(InputSystem::Key::E)) pos += _up * velocity;
            if (InputSystem::Input::IsKeyDown(InputSystem::Key::Q)) pos -= _up * velocity;

            SetPosition(pos);


        }

    private:
        void UpdateCameraVectors()
        {
            glm::vec3 front;
            front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
            front.y = sin(glm::radians(_pitch));
            front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
            _front = glm::normalize(front);

            // Recalculate Right and Up vector
            _right = glm::normalize(glm::cross(_front, glm::vec3(0.0f, 1.0f, 0.0f)));
            _up    = glm::normalize(glm::cross(_right, _front));

            // Tell the base Camera class to look along the new front vector
            SetOrientation(_front);
        }

        float _yaw = -90.0f; // initially, look down the -Z axis
        float _pitch = 0.0f;
        float _mouse_sensitivity = 0.1f;
        float _movement_speed = 5.0f;

        float _last_x = 0.0f, _last_y = 0.0f;
        bool _first_mouse = true;

        glm::vec3 _front    {0.0f, 0.0f, -1.0f};
        glm::vec3 _right    {1.0f, 0.0f, 0.0f };
        glm::vec3 _up       {0.0f, 1.0f, 0.0f };

    };
}