#pragma once

#include <vector>

#include "../Renderer/RenderEntity.hpp"
#include "../Scene/Camera.hpp"
#include "DemoCamera.hpp"
#include "DemoEntity.hpp"
#include "DemoEntity2.hpp"

namespace Inferonix::Project
{
    class Project
    {
    public:
        Project()
        {
            _main_camera = std::make_shared<DemoCamera>();
            AddEntityData<DemoEntity>();
            AddEntityData<DemoEntity2>();
        }

        [[nodiscard]] std::shared_ptr<Scene::Camera> GetMainCamera()
        {
            return _main_camera;
        }

        [[nodiscard]] std::vector<std::shared_ptr<Renderer::RenderEntityData>> const& GetEntitiesData() const
        {
            return _demo_entities;
        }

        template<typename T, typename... Args>
        T& AddEntityData(Args&&... args)
        {
            auto entity = std::make_shared<T>(std::forward<Args>(args)...);
            T& ref = *entity;
            _demo_entities.emplace_back(std::move(entity));
            return ref;
        }


        private:
        std::shared_ptr<DemoCamera> _main_camera;

        std::vector<std::shared_ptr<Renderer::RenderEntityData>> _demo_entities;

        // std::vector<std::shared_ptr<Scene::Camera>> _secondary_cameras;
    };

} // namespace Inferonix::Project
