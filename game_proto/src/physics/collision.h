#ifndef COLLISION_H
#define COLLISION_H

#include "../physics/body.h"
#include "../world/obstacle.h"

void ResolveCollision(Body *body, Obstacle *obstacle);

void ResolveAllCollisions(Body *body, Obstacle *obstacles, int count);

#endif 