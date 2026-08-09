#version 460 core

struct Particle {
    vec4 position;
    vec4 velocity;
};

layout(std430, binding = 0) buffer particle_buffer {
    Particle particles[];
};

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

uniform float delta_time;
uniform int total_particles;

void main() {
    uint i = gl_GlobalInvocationID.x;
    if (i >= total_particles)
        return;

    vec3 pos_i = particles[i].position.xyz;
    vec3 acc = vec3(0.0);

    for(uint j = 0; j < total_particles; j++) {
        if(i == j) continue;

        vec3 pos_j = particles[j].position.xyz;
        vec3 r = pos_j - pos_i;
        float distance = length(r);

        acc += particles[j].position.w * r * 0.1 / (distance * distance * distance);
    }

    particles[i].velocity.xyz += acc * delta_time;
    particles[i].position.xyz += particles[i].velocity.xyz * delta_time;
}



