#include "camera.h"

#include <algorithm>
#include <iostream>
#include <numbers>

#include "pmp/mat_vec.h"

pmp::mat4 Camera::getViewMatrix()
{
    return pmp::look_at_matrix(position, target, up);
}

void Camera::resetPosition()
{
    position = pmp::vec3(0.0f, 0.0f, distance);
    front = pmp::vec3(0.0f, 0.0f, -1.0f);
    target = pmp::vec3(0.0f);
}

void Camera::processMouse(float xOffset, float yOffset)
{
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw = fmod(yaw + xOffset, 360.f);
    pitch += yOffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    float rad = std::numbers::pi / 180.0f;
    pmp::vec3 direction;
    direction[0] = cos(yaw * rad) * cos(pitch * rad);
    direction[1] = sin(pitch * rad);
    direction[2] = sin(yaw * rad) * cos(pitch * rad);
    position = target + pmp::normalize(direction) * distance;
    front = pmp::normalize(position * -1.0f);
}

void Camera::processMove(float xPan, float yPan)
{
    xPan *= panSensitivity;
    yPan *= panSensitivity;

    pmp::vec3 right = pmp::normalize(pmp::cross(front, up));
    pmp::vec3 cameraUp = pmp::normalize(pmp::cross(right, front));
    target += right * -xPan + cameraUp * yPan;
    position += right * -xPan + cameraUp * yPan;
}

void Camera::addDistance(float offset)
{
    distance -= offset;
    distance = std::clamp(distance, minDistance, maxDistance);

    position = target + pmp::normalize(position - target) * distance;
}

void Camera::setSensitivity(int percentage)
{
    sensitivity = float(percentage) / 100.0f * (maxSensitivity - minSensitivity);
}

void Camera::screenPosToWorldRay(int mouseX, int mouseY, int screenWidth, int screenHeight, pmp::mat4 ViewMatrix,
                                 pmp::mat4 ProjectionMatrix, pmp::vec3& out_direction)
{
    // 1. Screen to NDC
    float ndcX = (2.0f * mouseX) / screenWidth - 1.0f;
    float ndcY = 1.0f - (2.0f * mouseY) / screenHeight;  // Flip Y for OpenGL NDC

    // 2. NDC to Clip Space (using near plane projection in NDC)
    pmp::vec4 ray_clip = pmp::vec4(ndcX, ndcY, -1.0f, 1.0f);

    // 3. Clip Space to View Space
    pmp::mat4 inverseProjection = pmp::inverse(ProjectionMatrix);
    pmp::vec4 ray_eye_h = inverseProjection * ray_clip;

    // Check if perspective divide is needed (W might not be 1)
    if (ray_eye_h[3] != 0.0f) {
        ray_eye_h /= ray_eye_h[3];
    } else {
        // Handle potential error or orthographic projection case
        // For standard perspective, w should not be 0 here.
        // Maybe set ray_eye_h.w = 1.0f if needed, although the math should work out.
        std::cout << "ERROR::CAMERA::SCREENPOSTOWORLDRAY::PERSPECTIVE_DIVIDE" << std::endl;
    }
    // The resulting ray_eye_h.xyz is now a point on the near plane in View Space.
    // The direction vector in View Space goes from the origin (camera) to this point.
    pmp::vec3 ray_view_dir = pmp::normalize(pmp::vec3(ray_eye_h[0], ray_eye_h[1], ray_eye_h[2]));

    // 4. View Space to World Space
    pmp::mat4 inverseView = pmp::inverse(ViewMatrix);
    // Transform the direction from View Space to World Space
    // Use w=0 for transforming directions
    pmp::vec4 ray_world_dir_h = inverseView * pmp::vec4(ray_view_dir, 0.0f);
    out_direction = pmp::normalize(pmp::vec3(ray_world_dir_h[0], ray_world_dir_h[1], ray_world_dir_h[2]));
}
