#include "Renderer.hpp"

#include "spdlog/spdlog.h"
#include "EventSystem/KeyEvent.hpp"
#include "Scene/Components.hpp"

#include <utility>

using namespace Inferonix::Renderer;

Renderer::Renderer(std::shared_ptr<Window::Window> window) : _window_instance(std::move(window))
{

    LogInfo();

    // todo: reserve place for vector?

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // for debugging
    glEnable(GL_DEBUG_OUTPUT);

    SetClearColor(0.1f, 0.5f, 0.7f, 1.0f);
}

void Renderer::Render(Scene::Scene const& scene)
{
    auto const delta_time = _window_instance->GetDeltaTime();
    _main_camera->Update(delta_time);

    auto view = scene.GetRegistry().view<Scene::MeshComponent, Scene::TransformComponent, Scene::ShaderComponent>();

    for (auto entity : view)
    {
        if (!_render_entities.contains(entity))
            CreateRenderEntity(entity, view.get<Scene::MeshComponent>(entity));

        auto& render_entity = _render_entities.at(entity);
        render_entity.shader_program.Use();
        render_entity.vertex_buffer.Bind();

        // auto& shader = view.get<Scene::ShaderComponent>(entity);
        // auto& transform = view.get<Scene::TransformComponent>(entity);

        // todo: uniforms
        // entity->shaderProgram->SetUniform("model", entity->renderEntityData->transform.GetMatrix());
        // entity->shaderProgram->SetUniform("view", _main_camera->GetView());
        // entity->shaderProgram->SetUniform("projection", _main_camera->GetProjection());

        glDrawElements(GL_TRIANGLES, static_cast<int>(render_entity.index_buffer.Count()), GL_UNSIGNED_INT, nullptr);
    }
}

void Renderer::CreateRenderEntity(entt::entity const& entity, Scene::MeshComponent const& mesh)
{
    RenderEntity render_entity{};

    render_entity.vertex_buffer = VertexBuffer{mesh.mesh_instance->GetVertices().size() * sizeof(Vertex)};

    render_entity.index_buffer = IndexBuffer
    {
        static_cast<signed long int>(mesh.mesh_instance->GetIndices().size() * sizeof(unsigned int)),
        mesh.mesh_instance->GetIndices().data()
    };

    render_entity.vertex_array.Bind();
    render_entity.vertex_buffer.Bind();

    render_entity.vertex_buffer.BufferData(
            mesh.mesh_instance->GetVertices().size() * sizeof(Vertex), mesh.mesh_instance->GetVertices().data()
    );

    render_entity.index_buffer.Bind();

    render_entity.index_buffer.BufferData(
        mesh.mesh_instance->GetIndices().size() * sizeof(unsigned int),
        mesh.mesh_instance->GetIndices().data()
    );

    VertexBufferLayout layout;
    layout.Push(ShaderDatatype::FLOAT, 3); // position
    layout.Push(ShaderDatatype::FLOAT, 3); // normal
    render_entity.vertex_array.AddVertexBuffer(render_entity.vertex_buffer, layout);

    /*
    render_entity.vertex_array.SetIndexBuffer(render_entity.index_buffer);
    render_entity.vertex_buffer.Unbind();
    render_entity.vertex_array.Unbind();
    */

    _render_entities[entity] = std::move{render_entity};


}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::LogInfo()
{

    spdlog::info("GPU vendor: {}", reinterpret_cast<char const*>(glGetString(GL_VENDOR)));
    spdlog::info("GPU Renderer: {}", reinterpret_cast<char const*>(glGetString(GL_RENDERER)));
    spdlog::info("GPU version: {}", reinterpret_cast<char const*>(glGetString(GL_VERSION)));
    spdlog::info("Shading language: {}", reinterpret_cast<char const*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    spdlog::info("Maximum number of vertex attributes supported: {}", nrAttributes);
}

void Renderer::OnEvent(EventSystem::Event& event)
{
    if (auto type = dynamic_cast<EventSystem::KeyEvent*>(&event))
    {
        if(type->GetKey() == InputSystem::Key::F1 && type->GetType() == EventSystem::KeyEventType::KeyPressedEvent)
        {
            _wireframe_mode = !_wireframe_mode;
            glPolygonMode(GL_FRONT_AND_BACK, _wireframe_mode ? GL_LINE : GL_FILL);
        }
    }
}
