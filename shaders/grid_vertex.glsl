#version 410 core

layout(location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;

out vec3 world_pos;

void main(){
    world_pos = aPos;
    gl_Position = projection * view * vec4(world_pos, 1.0);
}
