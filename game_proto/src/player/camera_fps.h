#ifndef CAMERA_FPS_H
#define CAMERA_FPS_H

#include "../player/player.h"
#include "raylib.h"

void UpdateCameraFPS(Camera *camera, Player *player);

void UpdateCameraFOV(Camera *camera, Player *player);

#endif