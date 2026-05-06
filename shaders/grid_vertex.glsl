#version 410 core

layout(location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;

out vec3 WorldPos;

void main(){
    WorldPos = aPos;
    gl_Position = projection * view * vec4(WorldPos, 1.0);
}
