#version 460 core

in vec3 normal;
in vec3 cameraDir;
in float selected;

out vec4 FragColor;

void main() {
    float d = dot(normal, cameraDir);
    if (d > -0.1) discard;

    float color = selected > 0.5 ? 0.9 : 0.15;

    FragColor = vec4(color, color, color, 1.0);
};
