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

void Renderer::Render(Scene::Scene& scene)
{
    //auto const delta_time = _window_instance->GetDeltaTime();
    //_main_camera->Update(delta_time);

    for (auto const view = scene.GetRegistry().view<Scene::MeshComponent, Scene::TransformComponent>(); auto entity : view)
    {
        auto const entity_index = static_cast<uint32_t>(entity);
        if (entity_index >= _render_entities.size() || !_render_entities[entity_index])
            CreateRenderEntity(entity, view.get<Scene::MeshComponent>(entity));

        auto const& render_entity = _render_entities[entity_index];
        render_entity->shader_program->Use();
        render_entity->vertex_array->Bind();

        render_entity->shader_program->SetUniform("myColor", 0.541f, 0.124f, 0.784f);

        // uniforms
        render_entity->shader_program->SetUniform(
            "model",
            view.get<Scene::TransformComponent>(entity).GetMatrix()
        );
        render_entity->shader_program.SetUniform("view", scene.GetMainCamera()->GetView());
        render_entity->shader_program.SetUniform("projection", scene.GetMainCamera()->GetProjection());

        glDrawElements(GL_TRIANGLES, static_cast<int>(render_entity->index_buffer->Count()), GL_UNSIGNED_INT, nullptr);

        render_entity->vertex_array->Unbind();
        render_entity->shader_program->Unuse();

    }
}

void Renderer::CreateRenderEntity(Scene::Entity const& entity, Scene::MeshComponent& mesh)
{
    auto render_entity = std::make_unique<RenderEntity>(

        std::make_unique<ShaderProgram>(),
        std::make_unique<VertexArray>(),
        std::make_unique<VertexBuffer>(),
        std::make_unique<IndexBuffer>
        (
            static_cast<long int>(mesh.GetIndices().size()),
            mesh.GetIndices().data()
        )
    );

    render_entity->vertex_array->Bind();
    render_entity->vertex_buffer->Bind();

    render_entity->vertex_buffer->BufferData(
            mesh.GetVertices().size() * sizeof(Vertex), mesh.GetVertices().data()
    );

    render_entity->index_buffer->Bind();

    render_entity->index_buffer->BufferData(
        mesh.GetIndices().size() * sizeof(unsigned int),
        mesh.GetIndices().data()
    );

    VertexBufferLayout layout;
    layout.Push(ShaderDatatype::FLOAT, 3); // position
    layout.Push(ShaderDatatype::FLOAT, 3); // normal
    render_entity->vertex_array->AddVertexBuffer(*render_entity->vertex_buffer, layout);


    render_entity->vertex_array->SetIndexBuffer(*render_entity->index_buffer);
    render_entity->vertex_buffer->Unbind();
    render_entity->vertex_array->Unbind();

    /**/

    auto const entity_index = static_cast<uint32_t>(entity);
    if (entity_index >= _render_entities.size())
        _render_entities.resize(entity_index + 1);

    _render_entities[static_cast<uint32_t>(entity)] = std::move(render_entity);

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
