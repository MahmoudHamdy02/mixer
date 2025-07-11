#include "pmp/mat_vec.h"

enum MovementDirection { FORWARD, BACK, RIGHT, LEFT };

class Camera
{
    float pitch = 25.0f;
    float yaw = 45.0f;
    float sensitivity = 0.2f;
    float minSensitivity = 0.05f;
    float maxSensitivity = 0.4f;
    float panSensitivity = 0.05;
    float minDistance = 5.0f;
    float maxDistance = 100.0f;
    pmp::vec3 up = pmp::vec3(0.0f, 1.0f, 0.0f);

public:
    float distance = 10.0f;
    pmp::vec3 position;
    pmp::vec3 front;
    pmp::vec3 target = pmp::vec3(0.0f);
    pmp::mat4 getViewMatrix();

    Camera();

    void resetPosition();
    void processMouse(float xOffset, float yOffset);
    void processMove(float xPan, float yPan);
    void addDistance(float distance);
    void setSensitivity(int percentage);

    static void screenPosToWorldRay(
        int mouseX, int mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
        int screenWidth, int screenHeight,  // Window size, in pixels
        pmp::mat4 ViewMatrix,               // Camera position and orientation
        pmp::mat4 ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
        pmp::vec3& out_direction  // Output : Direction, in world space, of the ray that goes "through" the mouse.
    );
};
