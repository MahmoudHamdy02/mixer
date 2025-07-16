#version 460 core

uniform mat4 view;
uniform mat4 projection;

in vec3 nearPoint;
in vec3 farPoint;

out vec4 fragColor;

vec4 grid(vec3 worldPos, float scale, bool drawAxis) {
    vec2 coord = worldPos.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;

    // High at line fragments only
    float lineOpacity = 1.0 - min(grid.x, grid.y);

    // High everywhere except for its axis
    float axisX = smoothstep(0.0, fwidth(worldPos.z), abs(worldPos.z));
    float axisZ = smoothstep(0.0, fwidth(worldPos.x), abs(worldPos.x));
    vec3 base = vec3(0.3);
    base.x = mix(1.0, base.x, axisX); // Red for X axis
    base.z = mix(1.0, base.z, axisZ); // Blue for Z axis

    return vec4(base, lineOpacity);
}

float computeDepth(vec3 pos) {
    vec4 clip_space_position = projection * view * vec4(pos.xyz, 1.0);
    // -1 -> 1 to 0 -> 1
    return 0.5 + 0.5 * (clip_space_position.z / clip_space_position.w);
}

float computeLinearDepth(float depth) {
    float near = 0.1;
    float far = 200.0;
    float linearDepth = (2.0 * near * far) / (far + near - depth * (far - near)); // Get linear value between 0.01 and 100
    return linearDepth / far; // Normalize to near/far -> 1.0
}

void main() {
    // Only render on the floor -> y = 0
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    if (t <= 0) discard;
    vec3 worldPos = nearPoint + t * (farPoint - nearPoint);

    float depth = computeDepth(worldPos);
    gl_FragDepth = depth;

    float linearDepth = computeLinearDepth(depth);
    float fading = max(0, (0.6 - linearDepth)); // Clamp to 0.5. Higher value -> less fading

    fragColor = grid(worldPos, 1, true);
    fragColor.a *= fading;
};
