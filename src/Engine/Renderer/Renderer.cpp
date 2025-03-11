#include "Renderer.hpp"

#include "spdlog/spdlog.h"
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

void Renderer::Render()
{
    auto delta_time = _window_instance->GetDeltaTime();
    _main_camera->Update(delta_time);

    for (auto const& entity : _render_entities)
    {
        entity->ShaderProgram->Use();
        entity->VertexArray->Bind();

        // todo: remove to Render entity properties
        if (entity->RenderEntityData->DynamicallyColored)
            entity->ShaderProgram->SetDynamicColor("myColor");
        else
            entity->ShaderProgram->SetUniform("myColor", 0.5f, 0.5f, 0.5f);

        entity->RenderEntityData->Update(delta_time);

        entity->ShaderProgram->SetUniform("model", entity->RenderEntityData->Transform.GetMatrix());
        entity->ShaderProgram->SetUniform("view", _main_camera->GetView());
        entity->ShaderProgram->SetUniform("projection", _main_camera->GetProjection());

        glDrawElements(GL_TRIANGLES, static_cast<int>(entity->IndexBuffer->Count()), GL_UNSIGNED_INT, nullptr);

        entity->VertexArray->Unbind();
        entity->ShaderProgram->Unuse();
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

    entity->RenderEntityData = std::move(data);

    entity->ShaderProgram = std::make_unique<ShaderProgram>();
    entity->VertexArray = std::make_unique<VertexArray>();

    entity->VertexBuffer = std::make_unique<VertexBuffer>(entity->RenderEntityData->Vertices.size() * sizeof(float));

    entity->IndexBuffer = std::make_unique<IndexBuffer>(
            entity->RenderEntityData->Indices.size() * sizeof(int),
            entity->RenderEntityData->Indices.data()
    );

    entity->VertexArray->Bind();
    entity->VertexBuffer->Bind();
    entity->VertexBuffer->BufferData(
            entity->RenderEntityData->Vertices.size() * sizeof(float),
            entity->RenderEntityData->Vertices.data()
    );

    entity->IndexBuffer->Bind();
    entity->IndexBuffer->BufferData(
            entity->RenderEntityData->Indices.size() * sizeof(int),
            entity->RenderEntityData->Indices.data()
    );

    VertexBufferLayout layout1;
    layout1.Push(ShaderDatatype::FLOAT, 3);
    entity->VertexArray->AddVertexBuffer(*entity->VertexBuffer, layout1);

    entity->VertexBuffer->Unbind();

    return entity;
}
