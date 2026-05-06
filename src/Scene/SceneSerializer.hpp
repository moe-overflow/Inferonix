#pragma once

#include "Scene.hpp"

namespace Inferonix::Scene
{

    class SceneSerializer final
    {
    private:
        Scene& _scene;

    public:
        explicit SceneSerializer(Scene& scene);

        [[nodiscard]] bool Serialize(const std::string& filepath) const;
        [[nodiscard]] bool Deserialize(const std::string& filepath) const;


    };

}

