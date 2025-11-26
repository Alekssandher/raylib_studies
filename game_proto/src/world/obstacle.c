#include "../world/obstacle.h"
#include "stdlib.h"
void InitObstacles(Obstacle **obstacles, int *count) {
    *count = 4;
    *obstacles = (Obstacle*)malloc(sizeof(Obstacle) * (*count));
    
    (*obstacles)[0] = (Obstacle){{ 16.0f, 16.0f,  16.0f}, {16.0f, 32.0f, 16.0f}};
    (*obstacles)[1] = (Obstacle){{-16.0f, 16.0f,  16.0f}, {16.0f, 32.0f, 16.0f}};
    (*obstacles)[2] = (Obstacle){{-16.0f, 16.0f, -16.0f}, {16.0f, 32.0f, 16.0f}};
    (*obstacles)[3] = (Obstacle){{ 16.0f, 16.0f, -16.0f}, {16.0f, 32.0f, 16.0f}};
}

void FreeObstacles(Obstacle *obstacles) {
    free(obstacles);
}

void DrawObstacle(Obstacle *obstacle) {
    DrawCube(obstacle->position, obstacle->size.x, obstacle->size.y, 
             obstacle->size.z, (Color){ 150, 200, 200, 255 });
    DrawCubeWires(obstacle->position, obstacle->size.x, obstacle->size.y, 
                  obstacle->size.z, DARKBLUE);
}