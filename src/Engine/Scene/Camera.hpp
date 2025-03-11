#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"

namespace Inferonix::Scene
{

    struct CameraSettings
    {

        float FOV = 45.0f;
        float AspectRatio = 16.0f / 9.0f;
        float NearPlane = 0.1f;
        float FarPlane = 100.0f;

        glm::vec3 Position = glm::vec3(0.0f, 0.0f, 5.0f);
        glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 UpVector = glm::vec3(0.0f, 1.0f, 0.0f);
    };

    class Camera
    {
    public:
        Camera() : Camera(CameraSettings()){};

        explicit Camera(CameraSettings const& settings) : _settings(settings)
        {
            UpdateProjection();
            UpdateView();
        }

        virtual void Update(float delta_time){};

        void SetPosition(glm::vec3 const& position)
        {
            _settings.Position = position;
            UpdateView();
        }

        void SetOrientation(glm::vec3 const& orientation)
        {
            _settings.Orientation = orientation;
            UpdateProjection();
        }

        [[nodiscard]] auto GetProjection() const
        {
            return _projection_matrix;
        }
        [[nodiscard]] auto GetView() const
        {
            return _view_matrix;
        }

        [[nodiscard]] auto GetSettings() const
        {
            return _settings;
        }

    private:
        void UpdateView()
        {
            _view_matrix =
                    glm::lookAt(_settings.Position, _settings.Position + _settings.Orientation, _settings.UpVector);
        }

        void UpdateProjection()
        {
            _projection_matrix = glm::perspective(
                    glm::radians(_settings.FOV),
                    _settings.AspectRatio,
                    _settings.NearPlane,
                    _settings.FarPlane
            );
        }


    private:
        CameraSettings _settings{};

        glm::mat4 _projection_matrix{};
        glm::mat4 _view_matrix{};
    };

} // namespace Inferonix::Scene
