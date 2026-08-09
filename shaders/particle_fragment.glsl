#version 460 core

layout (location = 0) out vec4 frag_color;
layout (location = 1) out int out_entity_id;

void main() {
    frag_color = vec4(0.0, 1.0, 1.0, 1.0);
    out_entity_id = -1;
}


