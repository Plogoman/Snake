#include <iostream>
#include "types.hpp"
#include "raylib.h"

int main()
{
    const i32 ScreenWidth = 860;
    const i32 ScreenHeight = 640;

    const Color Mint = {0, 255, 149, 255};

    InitWindow(ScreenWidth, ScreenHeight, "Snake");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Finally Got a Window", ScreenWidth / 3, ScreenHeight / 2, 30, Mint);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
