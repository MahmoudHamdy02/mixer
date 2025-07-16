#version 460 core

layout(location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;

out vec3 nearPoint;
out vec3 farPoint;

void main() {
    mat4 viewInv = inverse(view);
    mat4 projectionInv = inverse(projection);

    vec4 near = viewInv * projectionInv * vec4(aPos.xy, 0.0, 1.0);
    nearPoint = near.xyz / near.w;
    vec4 far = viewInv * projectionInv * vec4(aPos.xy, 1.0, 1.0);
    farPoint = far.xyz / far.w;

    gl_Position = vec4(aPos, 1.0);
};
