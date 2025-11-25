#include "raylib.h"
#include "rcamera.h"

typedef struct Player {

    Camera camera;
    Vector3 position;
    Vector3 size;
    Color color;
    float speed;
    bool colliding;

} Player;

void move_player(Player *player)
{
    if (IsKeyDown(KEY_D) ) player->position.x += player->speed;
    else if (IsKeyDown(KEY_A) ) player->position.x -= player->speed;

    if (IsKeyDown(KEY_S )) player->position.z += player->speed;
    else if (IsKeyDown(KEY_W) ) player->position.z -= player->speed;
}
BoundingBox GetPlayerBox(Player *p) {
    return (BoundingBox){
        .min = (Vector3){ p->position.x - p->size.x/2,
                          p->position.y - p->size.y/2,
                          p->position.z - p->size.z/2 },
        .max = (Vector3){ p->position.x + p->size.x/2,
                          p->position.y + p->size.y/2,
                          p->position.z + p->size.z/2 }
    };
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - box collisions");

    Vector3 enemyBoxPos = { -4.0f, 1.0f, 0.0f };
    Vector3 enemyBoxSize = { 2.0f, 2.0f, 2.0f };

    Vector3 enemySpherePos = { 4.0f, 0.0f, 0.0f };
    float enemySphereSize = 1.5f;

    SetTargetFPS(60);

    Player player = {
        .speed = 0.15f,
        .camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 },
        .position = { 0.0f, 1.0f, 2.0f },
        .size = { 1.0f, 2.0f, 1.0f },
        .color = GREEN,
        .colliding = false
    };

    
    while(!WindowShouldClose())
    {
        Vector3 oldPos = player.position;

        move_player(&player);

        player.colliding = false;

        if (CheckCollisionBoxes(
            GetPlayerBox(&player),
            (BoundingBox){(Vector3){ enemyBoxPos.x - enemyBoxSize.x/2,
                                     enemyBoxPos.y - enemyBoxSize.y/2,
                                     enemyBoxPos.z - enemyBoxSize.z/2 },
                          (Vector3){ enemyBoxPos.x + enemyBoxSize.x/2,
                                     enemyBoxPos.y + enemyBoxSize.y/2,
                                     enemyBoxPos.z + enemyBoxSize.z/2 }})) player.colliding = true;

        // Check collisions player vs enemy-sphere
        if (CheckCollisionBoxSphere(
            GetPlayerBox(&player),
            enemySpherePos, enemySphereSize)) player.colliding = true;
        
        if (player.colliding)
        {
            player.position = oldPos;  
            player.color = RED;
        }
        else player.color = GREEN;

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(player.camera);

                // Draw enemy-box
                DrawCube(enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, GRAY);

                DrawCubeWires(enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, DARKGRAY);

                float height = enemyBoxSize.y;
                float radius = enemyBoxSize.x / 2.0f;

                Vector3 p1 = (Vector3){
                    enemyBoxPos.x,
                    enemyBoxPos.y - height/2.0f,
                    enemyBoxPos.z
                };

                Vector3 p2 = (Vector3){
                    enemyBoxPos.x,
                    enemyBoxPos.y + height/2.0f,
                    enemyBoxPos.z
                };

                DrawCapsule(p1, p2, radius, 16, 32, RED);
                // Draw enemy-sphere
                DrawSphere(enemySpherePos, enemySphereSize, GRAY);
                DrawSphereWires(enemySpherePos, enemySphereSize, 16, 16, DARKGRAY);

                // Draw player
                DrawCubeV(player.position, player.size, player.color);

                DrawGrid(10, 1.0f);       

            EndMode3D();

            DrawText("Move player with arrow keys to collide", 220, 40, 20, GRAY);

            DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}