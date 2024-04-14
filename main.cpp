#include <deque>
#include <raylib.h>
#include <raymath.h>
#include "types.hpp"

const i32 CellSize = 30;
const i32 CellCount = 25;

f64 LastUpdateTime = 0;

const Color DarkSlateGray = {31, 64, 64, 255};
const Color MoonStone = {57, 162, 174, 255};

class Snake {
    public:
        deque<Vector2> Body = { Vector2 {6, 9}, Vector2 {5, 9}, Vector2 {4, 9}};
        Vector2 Direction = {1, 0};

        void Draw()
        {
            for (u32 i = 0; i < Body.size(); ++i)
            {
                f32 x = Body[i].x;
                f32 y = Body[i].y;
                Rectangle Segments = Rectangle{x * CellSize,y * CellSize, CellSize, CellSize};
                DrawRectangleRounded(Segments, 0.25, 6, DarkSlateGray);
            }
        }
        void Update()
        {
            Body.pop_back();
            Body.push_front(Vector2Add(Body[0], Direction));
        }
};
class Food {
    public:
        Vector2 Position;
        Texture2D Texture;

        Food()
        {
            Image Image = LoadImage("..\\Graphics\\Food.png");
            Texture = LoadTextureFromImage(Image);
            UnloadImage(Image);
            Position = GenerateRandomPosition();
        }
        ~Food()
        {
            UnloadTexture(Texture);
        }
        void Draw()
        {
            DrawTexture(Texture, Position.x * CellSize, Position.y * CellSize, WHITE);
        }
        Vector2 GenerateRandomPosition()
        {
            f32 x = GetRandomValue(0, CellCount - 1);
            f32 y = GetRandomValue(0, CellCount - 1);
            return Vector2 {x, y};
        }
};
class Game {
    public:
        Food Food;
        Snake Snake;

        void Draw()
        {
            Food.Draw();
            Snake.Draw();
        }
        void Update()
        {
            Snake.Update();
        }
        void CheckCollisionWithFood()
        {

        }
};

bool EventTriggered(f64 Interval)
{
    f64 CurrentTime = GetTime();
    if (CurrentTime - LastUpdateTime >= Interval)
    {
        LastUpdateTime = CurrentTime;
        return true;
    }
    return false;
}
int main()
{
    const i32 FPS = 60;

    InitWindow(CellSize * CellCount, CellSize * CellCount, "Snake");
    SetTargetFPS(FPS);

    Game Game;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        if (EventTriggered(0.2))
        {
            Game.Update();
        }
        if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && Game.Snake.Direction.y != 1)
        {
            Game.Snake.Direction = {0, -1};
        }
        if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && Game.Snake.Direction.y != -1)
        {
            Game.Snake.Direction = {0, 1};
        }
        if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && Game.Snake.Direction.x != 1)
        {
            Game.Snake.Direction = {-1, 0};
        }
        if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && Game.Snake.Direction.x != -1)
        {
            Game.Snake.Direction = {1, 0};
        }

        ClearBackground(MoonStone);
        Game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
