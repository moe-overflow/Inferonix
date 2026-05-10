#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"

namespace inferonix::scene
{

    struct camera_settings
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
        camera() : camera(camera_settings()){};

        virtual ~camera() = default;

        explicit camera(camera_settings const& settings) : _settings(settings)
        {
            update_projection();
            update_view();
        }

        virtual void update(float delta_time) {}

        virtual void set_block_input(bool block) {}

        void set_position(glm::vec3 const& position)
        {
            _settings.position = position;
            update_view();
        }

        void set_orientation(glm::vec3 const& orientation)
        {
            _settings.orientation = orientation;
            update_view();
        }

        [[nodiscard]] auto get_projection() const
        {
            return _projection_matrix;
        }
        [[nodiscard]] auto get_view() const
        {
            return _view_matrix;
        }

        [[nodiscard]] auto get_settings() const
        {
            return _settings;
        }

        [[nodiscard]] auto is_primary() const{ return _settings.is_primary; }

    private:
        void update_view()
        {
            _view_matrix = glm::lookAt(
                _settings.position,
                _settings.position + _settings.orientation,
                _settings.up_vector
            );
        }

        void update_projection()
        {
            _projection_matrix = glm::perspective(
                    glm::radians(_settings.fov),
                    _settings.aspect_ratio,
                    _settings.near_plane,
                    _settings.far_plane
            );
        }


    private:
        camera_settings _settings{};

        glm::mat4 _projection_matrix{};
        glm::mat4 _view_matrix{};

    protected:
        bool _block_input { false };
    };

}
