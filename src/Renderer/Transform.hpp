#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Inferonix::Renderer
{
    struct Transform
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        Transform() : position(0.0f), rotation(0.0f), scale(1.0f) { }


        [[nodiscard]] glm::mat4 GetMatrix() const
        {
            auto mat = glm::mat4(1.0f);
            mat = glm::translate(mat, position);
            mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            mat = glm::scale(mat, scale);
            return mat;
        }

        void Translate(glm::vec3 const& delta)
        {
            position += delta;
        }

        void Rotate(glm::vec3 const& delta)
        {
            rotation += delta;
        }

        void ScaleBy(glm::vec3 const& factor)
        {
            scale *= factor;
        }

        void NormalizeRotation()
        {
            rotation = glm::mod(rotation, 360.0f);
        }
    };

}
