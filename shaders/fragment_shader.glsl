#version 430 core

layout (location = 0) out vec4 fragment_color;
layout(location = 1) out int out_entity_id;

in vec2 texture_coordinates_o;

uniform float u_time;
uniform vec4 color;
uniform sampler2D albedo_map;
uniform bool use_texture;
uniform bool use_dynamic_color;
uniform int u_entity_id;


void main()
{
    out_entity_id = u_entity_id;

    if (use_texture) {
        fragment_color = texture(albedo_map, texture_coordinates_o);
    } else if(use_dynamic_color) {
        float red = sin(u_time) * 0.25 + 0.33;
        float green = cos(u_time) * 0.5 + 0.5;
        float blue = sin(u_time) * 0.5 + 0.5;
        fragment_color = vec4(red, green, blue, 1.0);
    } else {
        fragment_color = color;
    }
}


