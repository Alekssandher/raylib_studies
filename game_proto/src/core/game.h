#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "../player/player.h"
#include "../world/obstacle.h"

typedef struct {
    Player player;
    Camera camera;
    Obstacle *obstacles;
    int obstacleCount;
    Vector2 sensitivity;
    
    // Test box
    Vector3 boxPos;
    Vector3 boxSize;
} Game;

void InitGame(Game *game);

void UpdateGame(Game *game);

void RenderGame(Game *game);

void CleanupGame(Game *game);

#endif