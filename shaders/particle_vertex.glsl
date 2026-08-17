#version 460 core

struct Particle {
    vec4 position;
    vec4 velocity;
    vec4 age_lifetime;
};

layout(std430, binding = 0) buffer particle_buffer {
    Particle particles[];
};

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out float v_lifetime_ratio;

void main() {
    vec4 world_pos = model * vec4(particles[gl_VertexID].position.xyz, 1.0);
    gl_Position = projection * view * world_pos;

    float age = particles[gl_VertexID].age_lifetime.x;
    float lifetime = particles[gl_VertexID].age_lifetime.y;
    v_lifetime_ratio = clamp(age / max(lifetime, 0.001), 0.0, 1.0);

    float dist = length(gl_Position.xyz);
    gl_PointSize = max(1.0, 80.0 / max(dist, 0.1));
}
