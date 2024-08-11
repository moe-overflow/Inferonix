#include "renderer.hpp"

#include <spdlog/spdlog.h>

using namespace Inferonix::Renderer;

renderer::renderer() 
     : _render_entities(std::make_unique<std::vector<render_entity>>())
{
    log_info();
    

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // for debugging
    glEnable(GL_DEBUG_OUTPUT);

}

void renderer::render()
{
    for(const render_entity& entity : *_render_entities)
    {
        entity.ShaderProgram->use();
        entity.VertexArray->bind();
        entity.ShaderProgram->set_dynamic_color("myColor");
        glDrawElements(GL_TRIANGLES,
                      static_cast<int>(entity.IndexBuffer->count()),
                      GL_UNSIGNED_INT,
                      nullptr
        ); // todo indices => variable
        entity.VertexArray->unbind();
        entity.ShaderProgram->unuse();
    }

}

void renderer::set_render_entity(const std::vector<render_entity_data>& data)
{
    for (const auto& entity : data) {
        _render_entities->emplace_back(create_render_entity(entity));
    }
}

void renderer::set_clear_color(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer::log_info() {

    spdlog::info("GPU vendor: {}", reinterpret_cast<char const*> (glGetString(GL_VENDOR)));
    spdlog::info("GPU renderer: {}", reinterpret_cast<char const*> (glGetString(GL_RENDERER)));
    spdlog::info("GPU version: {}", reinterpret_cast<char const*> (glGetString(GL_VERSION)));
    spdlog::info("Shading language: {}", reinterpret_cast<char const*> (glGetString(GL_SHADING_LANGUAGE_VERSION)));

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    spdlog::info("Maximum number of vertex attributes supported: {}", nrAttributes);

}

std::unique_ptr<render_entity> renderer::create_render_entity(const render_entity_data& data){

    auto sp = std::make_unique<shader_program>();
    auto va = std::make_unique<vertex_array>();
    //std::make_unique<vertex_buffer>(sizeof(data.vertices), data.vertices),
    auto vb = std::make_unique<vertex_buffer>(data.vertices.size() * sizeof(float), data.vertices.data());
    auto ib = std::make_unique<index_buffer>(data.indices.size() * sizeof(int), data.indices.data());

    va->bind();
    vb->bind();
    vb->buffer_data(data.vertices.size() * sizeof(float), data.vertices.data()); // todo: redundant (see ctor)

    ib->bind();
    ib->buffer_data(data.indices.size() * sizeof(int), data.indices.data()); // todo: redundant (see ctor)

    vertex_buffer_layout layout1;
    layout1.push(shader_data_type::FLOAT, 3);
    va->add_vertex_buffer(*vb, layout1);

    vb->unbind();

    sp->set_uniform("myColor", 1.0f, 1.0f, 1.0f);

    return std::make_unique<render_entity>(std::move(sp), std::move(va), std::move(vb), std::move(ib));
}


