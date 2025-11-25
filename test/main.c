#include "raylib.h"
#include "rcamera.h"

#define MAX_COLUMNS 20

typedef struct Player {
    Camera camera;
    Vector3 position;
    Vector3 size;
    float speed;

} Player;

void configure_camera(Player *player)
{
    player->camera.position = (Vector3){ 0.0f, 2.0f, 6.0f };
    player->camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    player->camera.target = (Vector3){0.0f, 2.0f, 0.0f};
    player->camera.fovy = 60.f;
    player->camera.projection = CAMERA_PERSPECTIVE;

}
typedef struct WindowConfigs {
    int width;
    int height;
    const char *title;
} WindowConfigs;

#define WINDOW_CONFIG_DEFAULT (WindowConfigs){800, 450, "First Prototype"}

void draw(Player *player, Vector3*positions, float*heights, Color*colors)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(player->camera);

    DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, LIGHTGRAY);
    DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
    DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
    DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD); 

    for (int i = 0; i < MAX_COLUMNS; i++)
    {
        DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
        DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, MAROON);
    }

    DrawCube(player->camera.position, 0.5f, 0.5f, 0.5f, (Color){0, 0, 0});
    // DrawCubeWires(player->camera.position, 0.5f, 0.5f, 0.5f, DARKPURPLE);
    EndMode3D();

    EndDrawing();
}
int main()
{
    WindowConfigs win = WINDOW_CONFIG_DEFAULT;
    InitWindow(win.width, win.height, win.title);

    Player player = {
        .camera = 0,
        .position = (Vector3){0.0f, 2.0f, 0.0f},
        .speed = 0.2f
    };
    configure_camera(&player);

    float heights[MAX_COLUMNS] = { 0 };
    Vector3 positions[MAX_COLUMNS] = { 0 };
    Color colors[MAX_COLUMNS] = { 0 };

    for (int i = 0; i < MAX_COLUMNS; i++)
    {
        heights[i] = (float)GetRandomValue(1, 12);
        positions[i] = (Vector3){ (float)GetRandomValue(-15, 15), heights[i]/2.0f, (float)GetRandomValue(-15, 15) };
        colors[i] = (Color){ GetRandomValue(20, 255), GetRandomValue(10, 55), 30, 255 };
    }

    DisableCursor();
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        //UpdateCamera(&player.camera, CAMERA_FIRST_PERSON);
        // Collision area must chase camera
        UpdateCameraPro(&player.camera,
        (Vector3){
            (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))*0.1f -      // Move forward-backward
            (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))*0.1f,
            (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))*0.1f -   // Move right-left
            (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))*0.1f,
            0.0f                                                // Move up-down
        },
        (Vector3){
            GetMouseDelta().x*0.08f,                            // Rotation: yaw
            GetMouseDelta().y*0.08f,                            // Rotation: pitch
            0.0f                                                // Rotation: roll
        },
        GetMouseWheelMove()*2.0f); 

        draw(&player, positions, heights, colors);
    }

    CloseWindow();

    return 0;
}