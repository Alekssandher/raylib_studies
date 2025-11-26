#include "game.h"
#include "../config/constants.h"
#include "../physics/collision.h"
#include "../world/level.h"
#include "../player/camera_fps.h"

void InitGame(Game *game) {
    // Initialize player
    InitPlayer(&game->player);
    
    // Initialize camera
    game->camera.fovy = 60.0f;
    game->camera.projection = CAMERA_PERSPECTIVE;
    game->camera.position = (Vector3){
        game->player.body.position.x,
        game->player.body.position.y + (BOTTOM_HEIGHT + game->player.headLerp),
        game->player.body.position.z
    };
    game->camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    
    // Initialize obstacles
    InitObstacles(&game->obstacles, &game->obstacleCount);
    
    // Initialize sensitivity
    game->sensitivity = (Vector2){ 0.001f, 0.001f };
    
    // Initialize test box
    game->boxPos = (Vector3){ 5.0f, 1.0f, 0.0f };
    game->boxSize = (Vector3){ 2.0f, 2.0f, 2.0f };
}

void UpdateGame(Game *game) {
    // Handle mouse input
    Vector2 mouseDelta = GetMouseDelta();
    game->player.lookRotation.x -= mouseDelta.x * game->sensitivity.x;
    game->player.lookRotation.y += mouseDelta.y * game->sensitivity.y;
    
    // Handle keyboard input
    char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
    char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
    bool crouching = IsKeyDown(KEY_LEFT_CONTROL);
    bool jumping = IsKeyPressed(KEY_SPACE);
    
    // Update player
    UpdatePlayer(&game->player, sideway, forward, jumping, crouching);
    
    // Update physics
    UpdateBodyPhysics(&game->player.body, GetFrameTime());
    
    // Resolve collisions
    ResolveAllCollisions(&game->player.body, game->obstacles, game->obstacleCount);
    
    // Resolve test box collision
    Obstacle testBox = { game->boxPos, game->boxSize };
    ResolveCollision(&game->player.body, &testBox);
    
    // Check test box collision for rendering
    BoundingBox playerBox = GetBodyBoundingBox(&game->player.body);
    BoundingBox boxBounds = (BoundingBox){
        .min = { game->boxPos.x - game->boxSize.x/2, 
                 game->boxPos.y - game->boxSize.y/2, 
                 game->boxPos.z - game->boxSize.z/2 },
        .max = { game->boxPos.x + game->boxSize.x/2, 
                 game->boxPos.y + game->boxSize.y/2, 
                 game->boxPos.z + game->boxSize.z/2 }
    };
    game->player.body.isColliding = CheckCollisionBoxes(playerBox, boxBounds);
    
    // Update camera position
    game->camera.position = (Vector3){
        game->player.body.position.x,
        game->player.body.position.y + (BOTTOM_HEIGHT + game->player.headLerp),
        game->player.body.position.z
    };
    
    // Update camera FOV and orientation
    UpdateCameraFOV(&game->camera, &game->player);
    UpdateCameraFPS(&game->camera, &game->player);
}

void RenderGame(Game *game) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    BeginMode3D(game->camera);
    
    // Draw test box
    if (game->player.body.isColliding) {
        DrawCube(game->boxPos, game->boxSize.x, game->boxSize.y, 
                game->boxSize.z, RED);
        DrawCubeWires(game->boxPos, game->boxSize.x, game->boxSize.y, 
                     game->boxSize.z, BLACK);
    } else {
        DrawCube(game->boxPos, game->boxSize.x, game->boxSize.y, 
                game->boxSize.z, GREEN);
        DrawCubeWires(game->boxPos, game->boxSize.x, game->boxSize.y, 
                     game->boxSize.z, DARKGREEN);
    }
    
    // Draw level
    DrawLevel(game->obstacles, game->obstacleCount);
    
    EndMode3D();
    
    // Draw UI
    DrawRectangle(5, 5, 330, 75, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines(5, 5, 330, 75, BLUE);
    DrawText("Camera controls:", 15, 15, 10, BLACK);
    DrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
    DrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);
    
    Vector2 vel2D = (Vector2){ 
        game->player.body.velocity.x, 
        game->player.body.velocity.z 
    };
    DrawText(TextFormat("- Velocity Len: (%06.3f)", Vector2Length(vel2D)), 
             15, 60, 10, BLACK);
    
    EndDrawing();
}

void CleanupGame(Game *game) {
    FreeObstacles(game->obstacles);
}