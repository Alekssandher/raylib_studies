#include "../player/camera_fps.h"
#include "../config/constants.h"

void UpdateCameraFPS(Camera *camera, Player *player) {
    const Vector3 up = (Vector3){ 0.0f, 1.0f, 0.0f };
    const Vector3 targetOffset = (Vector3){ 0.0f, 0.0f, -1.0f };

    // Calculate yaw (left-right rotation)
    Vector3 yaw = Vector3RotateByAxisAngle(targetOffset, up, player->lookRotation.x);

    // Clamp pitch (up-down rotation)
    float maxAngleUp = Vector3Angle(up, yaw) - 0.001f;
    if (-(player->lookRotation.y) > maxAngleUp) {
        player->lookRotation.y = -maxAngleUp;
    }

    float maxAngleDown = Vector3Angle(Vector3Negate(up), yaw) * -1.0f + 0.001f;
    if (-(player->lookRotation.y) < maxAngleDown) {
        player->lookRotation.y = -maxAngleDown;
    }

    // Calculate pitch
    Vector3 right = Vector3Normalize(Vector3CrossProduct(yaw, up));
    float pitchAngle = -player->lookRotation.y - player->lean.y;
    pitchAngle = Clamp(pitchAngle, -PI / 2 + 0.0001f, PI / 2 - 0.0001f);
    Vector3 pitch = Vector3RotateByAxisAngle(yaw, right, pitchAngle);

    // Head animation
    float headSin = sinf(player->headTimer * PI);
    float headCos = cosf(player->headTimer * PI);
    const float stepRotation = 0.0015f;
    
    camera->up = Vector3RotateByAxisAngle(up, pitch, 
                                          headSin * stepRotation + player->lean.x);

    // Camera bobbing
    const float bobSide = 0.03f;
    const float bobUp = 0.05f;
    Vector3 bobbing = Vector3Scale(right, headSin * bobSide);
    bobbing.y = fabsf(headCos * bobUp);

    camera->position = Vector3Add(camera->position, 
                                  Vector3Scale(bobbing, player->walkLerp));
    camera->target = Vector3Add(camera->position, pitch);
}

void UpdateCameraFOV(Camera *camera, Player *player) {
    float delta = GetFrameTime();
    Body *body = &player->body;
    
    if (body->isGrounded && player->walkLerp > 0.5f) {
        camera->fovy = Lerp(camera->fovy, 55.0f, 5.0f * delta);
    } else {
        camera->fovy = Lerp(camera->fovy, 60.0f, 5.0f * delta);
    }
}