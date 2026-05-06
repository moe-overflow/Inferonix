#pragma once

#include "scene.hpp"

namespace Inferonix::Scene
{

    class scene_serializer final
    {
    private:
        scene& _scene;

    public:
        explicit scene_serializer(scene& scene);

        [[nodiscard]] bool Serialize(const std::string& filepath) const;
        [[nodiscard]] bool Deserialize(const std::string& filepath) const;


    };

}

