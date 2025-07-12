#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraDirection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Only render point if it is visible (+ small buffer for artifacting at 90 degrees)
    float d = dot(aNormal, cameraDirection);
    gl_PointSize = d < -0.1 ? 4.0 : 0.0;
};
