#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in float aSelected;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraDirection;

out vec3 normal;
out vec3 cameraDir;
out float selected;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Only render point if it is selected and visible (+ small buffer for artifacting at 90 degrees)
    // float d = dot(aNormal, cameraDirection);
    // gl_PointSize = d < -0.1 ? 4.0 : 0.0;
    gl_PointSize = 4.0;
    normal = aNormal;
    cameraDir = cameraDirection;
    selected = aSelected;
};
