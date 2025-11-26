#ifndef PLAYER_H
#define PLAYER_H

#include "../physics/body.h"
#include "raylib.h"

typedef struct {
    Body body;
    Vector2 lookRotation;
    float headTimer;
    float walkLerp;
    float headLerp;
    Vector2 lean;
} Player;

void InitPlayer(Player *player);

void UpdatePlayer(Player *player, char side, char forward, 
                  bool jumpPressed, bool crouchHold);

Body* GetPlayerBody(Player *player);

#endif 