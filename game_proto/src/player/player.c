#include "../player/player.h"
#include "../config/constants.h"

#ifdef LOGS
#include <stdio.h>
#endif

void InitPlayer(Player *player) {
    InitBody(&player->body, (Vector3){0, 0, 0}, (Vector3){1.0f, 3.0f, 1.0f});
    player->lookRotation = (Vector2){0, 0};
    player->headTimer = 0.0f;
    player->walkLerp = 0.0f;
    player->headLerp = STAND_HEIGHT;
    player->lean = (Vector2){0, 0};
}

void UpdatePlayer(Player *player, char side, char forward, 
                  bool jumpPressed, bool crouchHold) {
#ifdef LOGS
    printf("Char Side: %d\n", side);
    printf("Char Forward: %d\n", forward);
#endif

    Vector2 input = (Vector2){ (float)side, (float)-forward };

#if defined(NORMALIZE_INPUT)
    if ((side != 0) && (forward != 0)) {
        input = Vector2Normalize(input);
    }
#endif

    float delta = GetFrameTime();
    Body *body = &player->body;

    // Apply gravity
    if (!body->isGrounded) {
        body->velocity.y -= GRAVITY * delta;
    }

    // Jump
    if (body->isGrounded && jumpPressed) {
        body->velocity.y = JUMP_FORCE;
        body->isGrounded = false;
    }

    // Calculate movement direction
    float rot = player->lookRotation.x;
    Vector3 front = (Vector3){ sinf(rot), 0.0f, cosf(rot) };
    Vector3 right = (Vector3){ cosf(-rot), 0.0f, sinf(-rot) };

    Vector3 desiredDir = (Vector3){ 
        input.x * right.x + input.y * front.x, 
        0.0f, 
        input.x * right.z + input.y * front.z 
    };
    
    body->dir = Vector3Lerp(body->dir, desiredDir, CONTROL * delta);

    // Apply friction/air drag
    float decel = (body->isGrounded ? FRICTION : AIR_DRAG);
    Vector3 hvel = (Vector3){ 
        body->velocity.x * decel, 
        0.0f, 
        body->velocity.z * decel 
    };

    float hvelLength = Vector3Length(hvel);
    if (hvelLength < (NORMAL_SPEED * 0.01f)) {
        hvel = (Vector3){ 0 };
    }

    // Calculate acceleration
    float speed = Vector3DotProduct(hvel, body->dir);
    float sprint = IsKeyDown(KEY_LEFT_SHIFT) ? SPRINT_MULT : 1.0f;
    float maxSpeed = (crouchHold ? CROUCH_SPEED : NORMAL_SPEED * sprint);
    float accel = Clamp(maxSpeed - speed, 0.0f, MAX_ACCEL * delta);

    hvel.x += body->dir.x * accel;
    hvel.z += body->dir.z * accel;

    body->velocity.x = hvel.x;
    body->velocity.z = hvel.z;

    // Update head animations
    player->headLerp = Lerp(player->headLerp, 
                           (crouchHold ? CROUCH_HEIGHT : STAND_HEIGHT), 
                           20.0f * delta);

    if (body->isGrounded && ((forward != 0) || (side != 0))) {
        player->headTimer += delta * 3.0f;
        player->walkLerp = Lerp(player->walkLerp, 1.0f, 4.0f * delta);
    } else {
        player->walkLerp = Lerp(player->walkLerp, 0.0f, 4.0f * delta);
    }

    player->lean.x = Lerp(player->lean.x, side * 0.02f, LEAN_FORCE * delta);
    player->lean.y = Lerp(player->lean.y, forward * 0.015f, LEAN_FORCE * delta);
}

Body* GetPlayerBody(Player *player) {
    return &player->body;
}