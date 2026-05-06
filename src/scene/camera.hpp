#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"

namespace Inferonix::Scene
{

    struct CameraSettings
    {

        float fov = 45.0f;
        float aspect_ratio = 16.0f / 9.0f;
        float near_plane = 0.1f;
        float far_plane = 100.0f;

        bool is_primary = false;

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f);
        glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up_vector = glm::vec3(0.0f, 1.0f, 0.0f);
    };

    class camera
    {
    public:
        camera() : camera(CameraSettings()){};

        virtual ~camera() = default;

        explicit camera(CameraSettings const& settings) : _settings(settings)
        {
            UpdateProjection();
            UpdateView();
        }

        virtual void Update(float delta_time) {}

        void SetPosition(glm::vec3 const& position)
        {
            _settings.position = position;
            UpdateView();
        }

        void SetOrientation(glm::vec3 const& orientation)
        {
            _settings.orientation = orientation;
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

        [[nodiscard]] auto IsPrimary() const{ return _settings.is_primary; }

    private:
        void UpdateView()
        {
            _view_matrix =
                    glm::lookAt(
                        _settings.position,
                        _settings.position + _settings.orientation,
                        _settings.up_vector
                    );
        }

        void UpdateProjection()
        {
            _projection_matrix = glm::perspective(
                    glm::radians(_settings.fov),
                    _settings.aspect_ratio,
                    _settings.near_plane,
                    _settings.far_plane
            );
        }


    private:
        CameraSettings _settings{};

        glm::mat4 _projection_matrix{};
        glm::mat4 _view_matrix{};
    };

}
