#include "raylib.h"
#include "core/game.h"
#include "stdio.h"

Texture2D kuruminhaTexture;
Texture2D reiTexture;

Texture2D ResolveImage(char path[])
{
    Image img = LoadImage(path);
    ImageFlipVertical(&img);

    Texture2D tmpTxt = LoadTextureFromImage(img);

    UnloadImage(img);
    return tmpTxt;
}
int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, 
               "Game Prototype For Testing");
    
    Game game;
    
    kuruminhaTexture = ResolveImage("game_proto/assets/kuruminha-imp.jpg");

    reiTexture = ResolveImage("game_proto/assets/rei.png");

    InitGame(&game);

    DisableCursor();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateGame(&game);
        RenderGame(&game);
    }

    UnloadTexture(kuruminhaTexture);
    UnloadTexture(reiTexture);
    CleanupGame(&game);
    CloseWindow();

    return 0;
}