#version 460 core

in float v_lifetime_ratio;

layout (location = 0) out vec4 frag_color;
layout (location = 1) out int out_entity_id;

void main() {
    vec2 coord = gl_PointCoord * 2.0 - 1.0;
    float dist = dot(coord, coord);
    if (dist > 1.0)
        discard;

    float alpha = 1.0 - v_lifetime_ratio;
    frag_color = vec4(0.3, 0.78, 0.3, alpha); // todo: color to uniform
    out_entity_id = -1;
}
