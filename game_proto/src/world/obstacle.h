#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "raylib.h"

typedef struct {
    Vector3 position;
    Vector3 size;
} Obstacle;

extern Mesh   cubeMesh;
extern Model  texturedCube;
extern Model reiCube;

void InitObstacles(Obstacle **obstacles, int *count);

void FreeObstacles(Obstacle *obstacles);

void DrawObstacle(Obstacle *obstacle);
void InitResources();

#endif 