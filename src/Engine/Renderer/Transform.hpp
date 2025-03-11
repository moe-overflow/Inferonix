#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Inferonix::Renderer
{
    struct Transform
    {
        glm::vec3 Position;
        glm::vec3 Rotation;
        glm::vec3 Scale;

        Transform() : Position(0.0f), Rotation(0.0f), Scale(1.0f) { }


        [[nodiscard]] glm::mat4 GetMatrix() const
        {
            auto mat = glm::mat4(1.0f);
            mat = glm::translate(mat, Position);
            mat = glm::rotate(mat, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            mat = glm::rotate(mat, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            mat = glm::rotate(mat, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            mat = glm::scale(mat, Scale);
            return mat;
        }

        void Translate(glm::vec3 const& delta)
        {
            Position += delta;
        }

        void Rotate(glm::vec3 const& delta)
        {
            Rotation += delta;
        }

        void ScaleBy(glm::vec3 const& factor)
        {
            Scale *= factor;
        }

        void NormalizeRotation()
        {
            Rotation = glm::mod(Rotation, 360.0f);
        }
    };

} // namespace Inferonix::Renderer
