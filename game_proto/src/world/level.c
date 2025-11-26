#include "../world/level.h"
#include "raylib.h"

void DrawLevel(Obstacle *obstacles, int obstacleCount) {
    const int floorExtent = 25;
    const float tileSize = 5.0f;
    const Color tileColor1 = (Color){ 150, 200, 200, 255 };

    for (int y = -floorExtent; y < floorExtent; y++) {
        for (int x = -floorExtent; x < floorExtent; x++) {
            if ((y & 1) && (x & 1)) {
                DrawPlane((Vector3){ x * tileSize, 0.0f, y * tileSize }, 
                         (Vector2){ tileSize, tileSize }, tileColor1);
            } else if (!(y & 1) && !(x & 1)) {
                DrawPlane((Vector3){ x * tileSize, 0.0f, y * tileSize }, 
                         (Vector2){ tileSize, tileSize }, LIGHTGRAY);
            }
        }
    }

    for (int i = 0; i < obstacleCount; i++) {
        DrawObstacle(&obstacles[i]);
    }

    DrawSphere((Vector3){ 300.0f, 300.0f, 0.0f }, 100.0f, 
               (Color){ 255, 0, 0, 255 });
}