#version 460 core

uniform vec3 color;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

void main() {
    // vec3 ambient = vec3(0.2f, 0.2f, 0.2f);
    // vec3 lightPos = vec3(0.0f, 0.0f, 200.0f);
    // vec3 lightColor = vec3(0.8f, 0.8f, 0.8f);
    //
    // vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(lightPos - FragPos);
    //
    // float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = diff * lightColor;
    //
    // vec3 result = (ambient + diffuse) * color;
    FragColor = vec4(8.0, 0.0, 0.0, 1.0);
};
