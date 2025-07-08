#version 460 core

in vec3 nearPoint;
in vec3 farPoint;

out vec4 FragColor;

uniform mat4 view;
uniform mat4 projection;


vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) {
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    float axisX = smoothstep(0.0, fwidth(fragPos3D.z), abs(fragPos3D.z));
    float axisZ = smoothstep(0.0, fwidth(fragPos3D.x), abs(fragPos3D.x));
    vec3 base = vec3(0.2);
    base.x = mix(1.0, base.x, axisZ);  // Red for X axis
    base.z = mix(1.0, base.z, axisX);  // Blue for Z axis

    float alpha = 1.0 - min(line, 1.0);

    return vec4(base, alpha);
}

float computeDepth(vec3 pos) {
    vec4 clip_space_position = projection * view * vec4 (pos.xyz, 1.0);
    return 0.5 + 0.5 * (clip_space_position.z / clip_space_position.w);
}

float computeLinearDepth(vec3 pos) {
    float near = 0.1;
    float far = 100.0;
    vec4 clip_space_pos = projection * view * vec4(pos.xyz, 1.0);
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
    float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near)); // get linear value between 0.01 and 100
    return linearDepth / far; // normalize
}

void main() {
    // Only render on the floor -> y = 0
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    if (t <= 0) discard;
    vec3 xyPlanePos = nearPoint + t * (farPoint - nearPoint);
    gl_FragDepth = computeDepth(xyPlanePos);

    float linearDepth = computeLinearDepth(xyPlanePos);
    float fading = max(0, (0.5 - linearDepth));

    FragColor = grid(xyPlanePos, 1, true)* float(t > 0); // adding multiple resolution for the grid
    FragColor.a *= fading;
};
