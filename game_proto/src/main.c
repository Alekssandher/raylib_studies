#include "raylib.h"
#include "core/game.h"

int main(void) {
    const int screenWidth = 1600;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, 
               "raylib [core] example - 3d camera fps");
    
    Game game;
    InitGame(&game);
    
    DisableCursor();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateGame(&game);
        RenderGame(&game);
    }

    CleanupGame(&game);
    CloseWindow();

    return 0;
}