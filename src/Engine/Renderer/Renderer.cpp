#include "Renderer.hpp"

#include "spdlog/spdlog.h"
#include "EventSystem/KeyEvent.hpp"
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

void Renderer::Render() const
{
    auto const delta_time = _window_instance->GetDeltaTime();
    _main_camera->Update(delta_time);

    for (auto const& entity : _render_entities)
    {
        entity->shaderProgram->Use();
        entity->vertexArray->Bind();

        // todo: remove to Render entity properties
        if (entity->renderEntityData->dynamically_colored)
            entity->shaderProgram->SetDynamicColor("myColor");
        else
            entity->shaderProgram->SetUniform("myColor", 0.5f, 0.5f, 0.5f);

        entity->renderEntityData->Update(delta_time);

        entity->shaderProgram->SetUniform("model", entity->renderEntityData->transform.GetMatrix());
        entity->shaderProgram->SetUniform("view", _main_camera->GetView());
        entity->shaderProgram->SetUniform("projection", _main_camera->GetProjection());

        glDrawElements(GL_TRIANGLES, static_cast<int>(entity->indexBuffer->Count()), GL_UNSIGNED_INT, nullptr);

        entity->vertexArray->Unbind();
        entity->shaderProgram->Unuse();
    }
}

void Renderer::AddRenderEntity(std::shared_ptr<RenderEntityData> const& data)
{
    _render_entities.emplace_back(CreateRenderEntity(data));
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

std::shared_ptr<RenderEntity> Renderer::CreateRenderEntity(std::shared_ptr<RenderEntityData> data)
{
    auto entity = std::make_shared<RenderEntity>();

    entity->renderEntityData = std::move(data);

    entity->shaderProgram = std::make_unique<ShaderProgram>();

    /**/

    entity->vertexArray = std::make_unique<VertexArray>();

    entity->vertexBuffer = std::make_unique<VertexBuffer>(entity->renderEntityData->mesh_instance->GetVertices().size() * sizeof(Vertex));

    entity->indexBuffer = std::make_unique<IndexBuffer>(
        entity->renderEntityData->mesh_instance->GetIndices().size() * sizeof(unsigned int),
        entity->renderEntityData->mesh_instance->GetIndices().data()
    );

    /**/

    entity->vertexArray->Bind();
    entity->vertexBuffer->Bind();

    entity->vertexBuffer->BufferData(
            entity->renderEntityData->mesh_instance->GetVertices().size() * sizeof(Vertex),
            entity->renderEntityData->mesh_instance->GetVertices().data()
    );

    entity->indexBuffer->Bind();

    entity->indexBuffer->BufferData(
            entity->renderEntityData->mesh_instance->GetIndices().size() * sizeof(unsigned int),
            entity->renderEntityData->mesh_instance->GetIndices().data()
    );

    VertexBufferLayout layout;
    layout.Push(ShaderDatatype::FLOAT, 3); // position
    layout.Push(ShaderDatatype::FLOAT, 3); // normal
    entity->vertexArray->AddVertexBuffer(*entity->vertexBuffer, layout);

    entity->vertexArray->SetIndexBuffer(*entity->indexBuffer);

    entity->vertexBuffer->Unbind();
    entity->vertexArray->Unbind();

    return entity;
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
