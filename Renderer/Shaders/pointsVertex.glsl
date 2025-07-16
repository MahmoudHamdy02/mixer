#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aInstancePos;
layout(location = 2) in vec3 aInstanceNormal;
layout(location = 3) in float aInstanceSelected;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out float selected;

void main() {
    gl_Position = projection * view * model * vec4(aPos + aInstancePos, 1.0);

    normal = aInstanceNormal;
    selected = aInstanceSelected;
};
