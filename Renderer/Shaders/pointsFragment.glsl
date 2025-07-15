#version 460 core

uniform vec3 cameraDirection;

in vec3 normal;
in float selected;

out vec4 FragColor;

void main() {
    float d = dot(normal, cameraDirection);
    if (d > -0.1) discard;

    float color = selected > 0.5 ? 0.9 : 0.15;

    FragColor = vec4(color, color, color, 1.0);
};
