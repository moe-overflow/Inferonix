#version 430 core
layout (location = 0) out vec4 color;

in vec2 texture_coordinates_o;

uniform vec4 my_color;
uniform sampler2D albedo_map;
uniform bool use_texture;

void main()
{
    if (use_texture) {
        color = texture(albedo_map, texture_coordinates_o);
    } else {
        color = my_color;
    }
}

