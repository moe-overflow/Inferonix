#version 410 core


layout (location = 0) out vec4 fragment_color;
layout (location = 1) out int out_entity_id;

in vec3 world_pos;

void main() {
    // disallow selecting the grid
    out_entity_id = -1;

    // calculate grid lines
    vec2 coordinates = world_pos.xz;
    vec2 grid = abs(fract(coordinates - 0.5) - 0.5) / fwidth(coordinates);
    float line = min(grid.x, grid.y);

    // base transparency for the lines
    float alpha = 1.0 - min(line, 1.0);

    // fade out the grid cleanly in the distance (max distance: 20)
    float dist = length(world_pos.xz);
    alpha *= max(0.0, 1.0 - (dist / 20.0));

    // base color (grey)
    vec3 color = vec3(0.3);

    // highlight the main axes (X: red, Z:blue)
    if (abs(world_pos.x) < 0.05) color = vec3(0.2, 0.2, 1.0); // Z-Axis
    if (abs(world_pos.z) < 0.05) color = vec3(1.0, 0.2, 0.2); // X-Axis

    // skip invisible pixels
    if (alpha < 0.01) discard;

    fragment_color = vec4(color, alpha);
}

