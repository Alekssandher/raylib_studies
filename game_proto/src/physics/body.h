#ifndef BODY_H
#define BODY_H

#include "raylib.h"
#include "raymath.h"

typedef struct {
    Vector3 position;
    Vector3 size;
    Vector3 velocity;
    Vector3 dir;
    bool isGrounded;
    bool isColliding;
} Body;

void InitBody(Body *body, Vector3 position, Vector3 size);

void UpdateBodyPhysics(Body *body, float delta);

BoundingBox GetBodyBoundingBox(Body *body);

#endif // BODY_H