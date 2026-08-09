#version 460 core

struct Particle {
    vec4 position;
    vec4 velocity;
};

layout(std430, binding = 0) buffer particle_buffer {
    Particle particles[];
};

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * model * vec4(particles[gl_VertexID].position.xyz, 1.0);
    gl_PointSize = 2.0; //  todo: make uniform out of this
}


