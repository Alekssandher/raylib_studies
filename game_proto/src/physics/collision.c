#include "collision.h"

void ResolveCollision(Body *body, Obstacle *obstacle) {
    BoundingBox pBox = GetBodyBoundingBox(body);
    BoundingBox bBox = (BoundingBox){obstacle->position, obstacle->position};
    
    bBox.min.x -= obstacle->size.x / 2;
    bBox.min.y -= obstacle->size.y / 2;
    bBox.min.z -= obstacle->size.z / 2;
    bBox.max.x += obstacle->size.x / 2;
    bBox.max.y += obstacle->size.y / 2;
    bBox.max.z += obstacle->size.z / 2;

    if (!CheckCollisionBoxes(pBox, bBox)) return;

    float px1 = bBox.max.x - pBox.min.x;
    float px2 = pBox.max.x - bBox.min.x;
    float py1 = bBox.max.y - pBox.min.y;
    float py2 = pBox.max.y - bBox.min.y;
    float pz1 = bBox.max.z - pBox.min.z;
    float pz2 = pBox.max.z - bBox.min.z;

    float minX = (px1 < px2) ? px1 : -px2;
    float minY = (py1 < py2) ? py1 : -py2;
    float minZ = (pz1 < pz2) ? pz1 : -pz2;

    float absX = fabsf(minX);
    float absY = fabsf(minY);
    float absZ = fabsf(minZ);

    if (absX < absY && absX < absZ) {
        body->position.x += minX;
    } else if (absY < absX && absY < absZ) {
        body->position.y += minY;
        body->velocity.y = 0;
        body->isGrounded = (minY > 0);
    } else {
        body->position.z += minZ;
    }
}

void ResolveAllCollisions(Body *body, Obstacle *obstacles, int count) {
    for (int i = 0; i < count; i++) {
        ResolveCollision(body, &obstacles[i]);
    }
}