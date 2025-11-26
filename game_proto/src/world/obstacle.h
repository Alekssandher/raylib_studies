#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "raylib.h"

typedef struct {
    Vector3 position;
    Vector3 size;
} Obstacle;

void InitObstacles(Obstacle **obstacles, int *count);

void FreeObstacles(Obstacle *obstacles);

void DrawObstacle(Obstacle *obstacle);

#endif 