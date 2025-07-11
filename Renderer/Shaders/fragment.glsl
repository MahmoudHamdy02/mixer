#version 460 core

uniform vec3 cameraDirection;

in vec3 normal;
in vec3 FragPos;

out vec4 FragColor;

void main() {
    // Normalize the dot product from -1 -> 1 to 0 -> 1
    // so that perpendicular faces are 0.5 instead of 0
    float lightStrength = 0.5 + 0.5 * dot(normal, normalize(-cameraDirection));
    vec3 color = lightStrength * vec3(0.8); // Cap brightest color to 0.8

    FragColor = vec4(color, 1.0);
};
