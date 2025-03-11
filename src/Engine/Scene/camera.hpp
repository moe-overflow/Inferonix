#pragma once

#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Inferonix::Scene
{

    struct camera_settings
    {

        float _fov = 45.0f;
        float _aspect_ratio = 16.0f / 9.0f;
        float _near_plane = 0.1f;
        float _far_plane = 100.0f;

        glm::vec3 _position = glm::vec3(0.0f, 0.0f, 5.0f);
        glm::vec3 _orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 _up_vector = glm::vec3(0.0f, 1.0f, 0.0f);

    };

    class camera
    {
    public:
        camera() : camera(camera_settings())
        { };

        explicit camera(const camera_settings& settings)
            : _settings(settings)
        {
            update_projection();
            update_view();
        }

        virtual void update(float delta_time) {};

        void set_position(const glm::vec3& position)
        {
            _settings._position = position;
            update_view();
        }

        void set_orientation(const glm::vec3& orientation)
        {
            _settings._orientation = orientation;
            update_projection();
        }

        [[nodiscard]] auto get_projection() const { return _projection_matrix; }
        [[nodiscard]] auto get_view() const { return _view_matrix; }

        [[nodiscard]] auto get_settings() const { return _settings; }

    private:

        void update_view()
        {
            _view_matrix = glm::lookAt(
                    _settings._position,
                    _settings._position + _settings._orientation,
                    _settings._up_vector
            );
        }

        void update_projection()
        {
            _projection_matrix = glm::perspective(
                    glm::radians(_settings._fov),
                    _settings._aspect_ratio,
                    _settings._near_plane,
                    _settings._far_plane
            );
        }


    private:

        camera_settings _settings{};

        glm::mat4 _projection_matrix{};
        glm::mat4 _view_matrix{};

    };

}
