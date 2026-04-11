#include "Renderer.hpp"

#include "spdlog/spdlog.h"
#include "EventSystem/KeyEvent.hpp"
#include "Scene/Components.hpp"

#include <utility>

using namespace Inferonix::Renderer;

Renderer::Renderer(std::shared_ptr<Window::Window> window) : _window_instance(std::move(window))
{
#ifndef NDEBUG
    SetupOpenGLDebug();
#endif

    SetDeviceSpecs();
    SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    SetupGrid();
}

void Renderer::Render(Scene::Scene& scene)
{

    if (_grid)
    {

        // Alpha Blending (for handling transparency)
        {
            // calculate a color by mixing the new pixel with pixel already in buffer
            glEnable(GL_BLEND);

            // use the alpha value of new color to determine opacity
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        }

        _grid->shader_program.Use();
        _grid->vertex_array.Bind();

        _grid->shader_program.SetUniform("view", scene.GetMainCamera()->GetView());
        _grid->shader_program.SetUniform("projection", scene.GetMainCamera()->GetProjection());

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        _grid->vertex_array.Unbind();
        _grid->shader_program.Unuse();

        glDisable(GL_BLEND);

    }

    //auto const delta_time = _window_instance->GetDeltaTime();
    //_main_camera->Update(delta_time);

    for (auto const view = scene.GetRegistry().view<Scene::MeshComponent, Scene::TransformComponent>(); auto entity : view)
    {
        auto const entity_index = static_cast<uint32_t>(entity);
        if (entity_index >= _render_entities.size() || !_render_entities[entity_index])
            CreateRenderEntity(entity, view.get<Scene::MeshComponent>(entity));

        auto const& render_entity = _render_entities[entity_index];
        render_entity->shader_program.Use();
        render_entity->vertex_array.Bind();

        render_entity->shader_program.SetUniform("myColor", 0.541f, 0.124f, 0.784f);

        // uniforms
        render_entity->shader_program.SetUniform(
            "model",
            view.get<Scene::TransformComponent>(entity).GetMatrix()
        );
        render_entity->shader_program.SetUniform("view", scene.GetMainCamera()->GetView());
        render_entity->shader_program.SetUniform("projection", scene.GetMainCamera()->GetProjection());

        glDrawElements(GL_TRIANGLES, static_cast<int>(render_entity->index_buffer.Count()), GL_UNSIGNED_INT, nullptr);

        render_entity->vertex_array.Unbind();
        render_entity->shader_program.Unuse();

    }
}

void Renderer::CreateRenderEntity(Scene::Entity const& entity, Scene::MeshComponent& mesh)
{
    auto render_entity = std::make_unique<RenderEntity>();

    render_entity->vertex_array.Bind();
    render_entity->vertex_buffer.Bind();

    render_entity->vertex_buffer.BufferData(mesh.GetVertices());

    render_entity->index_buffer.Bind();
    render_entity->index_buffer.BufferData(mesh.GetIndices());

    VertexBufferLayout layout;
    layout.Push(ShaderDatatype::FLOAT, 3); // position
    layout.Push(ShaderDatatype::FLOAT, 3); // normal
    render_entity->vertex_array.AddVertexBuffer(render_entity->vertex_buffer, layout);


    render_entity->vertex_array.SetIndexBuffer(render_entity->index_buffer);
    render_entity->vertex_buffer.Unbind();
    render_entity->vertex_array.Unbind();

    /**/

    if (auto const entity_index = static_cast<uint32_t>(entity); entity_index >= _render_entities.size())
        _render_entities.resize(entity_index + 1);

    _render_entities[static_cast<uint32_t>(entity)] = std::move(render_entity);

}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetDeviceSpecs()
{
    _device_specs.vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    _device_specs.renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    _device_specs.version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    _device_specs.shading_language_version = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    int max_vertex_attributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attributes);
    _device_specs.nr_attributes = max_vertex_attributes;

    spdlog::info("Device Specifications: \n vendor: {}, \n renderer: {}, \n version: {}, \n shading language version: {}",
        _device_specs.vendor, _device_specs.renderer, _device_specs.version, _device_specs.shading_language_version);
}

void Renderer::SetupOpenGLDebug()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback([] (
        [[maybe_unused]] GLenum source,
        [[maybe_unused]] GLenum type,
        [[maybe_unused]] GLuint id,
                         GLenum const severity,
        [[maybe_unused]] GLsizei length,
                         const GLchar* message,
        [[maybe_unused]] const void* userParam) -> void
        {
            if (severity == GL_DEBUG_SEVERITY_MEDIUM)
                spdlog::warn("OpenGL warning({}): {}", id, message);
            else if (severity == GL_DEBUG_SEVERITY_HIGH)
                spdlog::error("OpenGL error({}): {}", id, message);
        },
        nullptr
    );
}

void Renderer::OnEvent(EventSystem::Event& event)
{
    if (auto const type = dynamic_cast<EventSystem::KeyEvent*>(&event))
    {
        if(type->GetKey() == InputSystem::Key::F1 && type->GetType() == EventSystem::KeyEventType::KeyPressedEvent)
        {
            _wireframe_mode = !_wireframe_mode;
            glPolygonMode(GL_FRONT_AND_BACK, _wireframe_mode ? GL_LINE : GL_FILL);
        }
    }
}

void Renderer::SetupGrid()
{
    _grid = std::make_unique<RenderEntity>();
    _grid->shader_program = ShaderProgram{
        SHADERS_PATH "/GridVertex.glsl",
        SHADERS_PATH "/GridFragment.glsl"
    };

    auto const vertices = std::vector<Vertex>{
        {{-50.0f, 0.0f, -50.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 50.0f, 0.0f, -50.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 50.0f, 0.0f,  50.0f}, {0.0f, 1.0f, 0.0f}},
        {{-50.0f, 0.0f,  50.0f}, {0.0f, 1.0f, 0.0f}}
    };
    auto indices = std::vector<uint32_t> { 0, 1, 2, 2, 3, 0 };


    _grid->vertex_buffer.Bind();
    _grid->vertex_buffer.BufferData(vertices);

    _grid->index_buffer.Bind();
    _grid->index_buffer.BufferData(indices);

    VertexBufferLayout layout;
    layout.Push(FLOAT, 3); // position
    layout.Push(FLOAT, 3); // normal

    _grid->vertex_array.Bind();
    _grid->vertex_array.AddVertexBuffer(_grid->vertex_buffer, layout);
    _grid->vertex_array.SetIndexBuffer(_grid->index_buffer);

    _grid->vertex_array.Unbind();
}



