#version 460 core

struct Particle {
    vec4 position;
    vec4 velocity;
    vec4 age_lifetime; // x = current age, y = max lifetime
};

layout(std430, binding = 0) buffer particle_buffer {
    Particle particles[];
};

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

uniform float delta_time;
uniform int total_particles;
uniform float drag;

float random(float x) {
    return fract(sin(x) * 62453.3253);
}

void main() {
    uint i = gl_GlobalInvocationID.x;
    if (i >= total_particles)
        return;

    vec3 pos = particles[i].position.xyz;
    vec3 vel = particles[i].velocity.xyz;

    float age = particles[i].age_lifetime.x + delta_time;
    float lifetime = particles[i].age_lifetime.y;

    if (age >= lifetime) {
        age = 0.0;
        pos = vec3(0.0);
        vel = vec3(
            random(float(i) * 12) * 0.4 - 0.2,
            random(float(i) * 78) * 0.4 + 0.1,
            random(float(i) * 45) * 0.4 - 0.2
        );
    }

    vec3 acc = vec3(0.0, 0.1, 0.0);

    // N-body gravity
    for(uint j = 0; j < uint(total_particles); j++) {
        if(i == j) continue;

        vec3 r = particles[j].position.xyz - pos;
        float dist = length(r);
        if (dist < 0.01) continue;
        acc += particles[j].position.w * r * 0.1 / (dist * dist * dist);
    }

    vel += acc * delta_time;
    vel *= max(0.0, 1.0 - drag * delta_time);
    pos += vel * delta_time;

    particles[i].position.xyz = pos;
    particles[i].velocity.xyz = vel;
    particles[i].age_lifetime.x = age;
}
