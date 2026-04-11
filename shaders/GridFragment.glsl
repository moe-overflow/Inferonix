#version 410 core
out vec4 FragColor;
in vec3 WorldPos;

void main() {
    // calculate grid lines
    vec2 coordinates = WorldPos.xz;
    vec2 grid = abs(fract(coordinates - 0.5) - 0.5) / fwidth(coordinates);
    float line = min(grid.x, grid.y);

    // base transparency for the lines
    float alpha = 1.0 - min(line, 1.0);

    // fade out the grid cleanly in the distance (max distance: 20)
    float dist = length(WorldPos.xz);
    alpha *= max(0.0, 1.0 - (dist / 20.0));

    // base color (grey)
    vec3 color = vec3(0.3);

    // highlight the main axes (X: red, Z:blue)
    if (abs(WorldPos.x) < 0.05) color = vec3(0.2, 0.2, 1.0); // Z-Axis
    if (abs(WorldPos.z) < 0.05) color = vec3(1.0, 0.2, 0.2); // X-Axis

    // skip invisible pixels
    if (alpha < 0.01) discard;

    FragColor = vec4(color, alpha);
}

