#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace inferonix::renderer
{
    struct transform
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        transform() : position(0.0f), rotation(0.0f), scale(1.0f) { }


        [[nodiscard]] glm::mat4 get_matrix() const
        {
            auto mat = glm::mat4(1.0f);
            mat = glm::translate(mat, position);
            mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            mat = glm::scale(mat, scale);
            return mat;
        }

        void translate(glm::vec3 const& delta)
        {
            position += delta;
        }

        void rotate(glm::vec3 const& delta)
        {
            rotation += delta;
        }

        void scale_by(glm::vec3 const& factor)
        {
            scale *= factor;
        }

        void normalize_rotation()
        {
            rotation = glm::mod(rotation, 360.0f);
        }
    };

}
