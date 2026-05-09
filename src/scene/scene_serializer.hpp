#pragma once

#include "scene.hpp"

namespace inferonix::scene
{

    class scene_serializer final
    {
    private:
        scene& _scene;

    public:
        explicit scene_serializer(scene& scene);

        [[nodiscard]] auto serialize(const std::string& filepath) const -> bool;

        auto deserialize(const std::string& filepath) const -> void;


    };

}

