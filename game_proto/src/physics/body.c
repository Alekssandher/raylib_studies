#include "../physics/body.h"
#include "../config/constants.h"

void InitBody(Body *body, Vector3 position, Vector3 size) {
    body->position = position;
    body->size = size;
    body->velocity = (Vector3){ 0.0f, 0.0f, 0.0f };
    body->dir = (Vector3){ 0.0f, 0.0f, 0.0f };
    body->isGrounded = false;
    body->isColliding = false;
}

void UpdateBodyPhysics(Body *body, float delta) {
    // Apply gravity
    if (!body->isGrounded) {
        body->velocity.y -= GRAVITY * delta;
    }

    // Update position based on velocity
    body->position.x += body->velocity.x * delta;
    body->position.y += body->velocity.y * delta;
    body->position.z += body->velocity.z * delta;

    // Floor collision
    if (body->position.y <= 0.0f) {
        body->position.y = 0.0f;
        body->velocity.y = 0.0f;
        body->isGrounded = true;
    }
}

BoundingBox GetBodyBoundingBox(Body *body) {
    return (BoundingBox){
        .min = (Vector3){ 
            body->position.x - body->size.x / 2,
            body->position.y - body->size.y / 2,
            body->position.z - body->size.z / 2 
        },
        .max = (Vector3){ 
            body->position.x + body->size.x / 2,
            body->position.y + body->size.y / 2,
            body->position.z + body->size.z / 2 
        }
    };
}