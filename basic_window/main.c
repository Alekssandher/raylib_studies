#include "raylib.h"

int main(void)
{
    const int screenW = 800;
    const int screenH = 450;

    InitWindow(screenW, screenH, "ralib [core] Basic Example");

    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText("My first raylib program", 190, 200, 20, LIGHTGRAY);

        EndDrawing();


    }

    CloseWindow();

    return 0;
}