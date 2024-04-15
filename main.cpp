#include <deque>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "types.hpp"

const i32 CellSize = 30;
const i32 CellCount = 25;
const i32 Offset = 75;

f64 LastUpdateTime = 0;

const Color DarkSlateGray = {31, 64, 64, 255};
const Color MoonStone = {57, 162, 174, 255};

bool ElementInDeque(Vector2 Element, deque<Vector2> Deque)
{
    for (int i = 0; i < Deque.size(); ++i)
    {
        if (Vector2Equals(Deque[i], Element))
        {
            return true;
        }
    }
    return false;
}
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

class Snake {
    public:
        deque<Vector2> Body = { Vector2 {6, 9}, Vector2 {5, 9}, Vector2 {4, 9}};
        Vector2 Direction = {1, 0};
        bool AddSegment = false;

        void Draw()
        {
            for (u32 i = 0; i < Body.size(); ++i)
            {
                f32 x = Body[i].x;
                f32 y = Body[i].y;
                Rectangle Segments = Rectangle{ Offset + x * CellSize,Offset + y * CellSize, (f32)CellSize, (f32)CellSize};
                DrawRectangleRounded(Segments, 0.25, 6, DarkSlateGray);
            }
        }
        void Update()
        {
            Body.push_front(Vector2Add(Body[0], Direction));
            if (AddSegment == true) {
                AddSegment = false;
            } else {
                Body.pop_back();
            }
        }
        void Reset()
        {
            Body = { Vector2 {6, 9}, Vector2 {5, 9}, Vector2 {4, 9}};
            Direction = {1, 0};
        }
};
class Food {
    public:
        Vector2 Position;
        Texture2D Texture;

        Food(deque<Vector2> SnakeBody)
        {
            Image Image = LoadImage("..\\Graphics\\Food.png");
            Texture = LoadTextureFromImage(Image);
            UnloadImage(Image);
            Position = GenerateRandomPosition(SnakeBody);
        }
        ~Food()
        {
            UnloadTexture(Texture);
        }
        void Draw()
        {
            DrawTexture(Texture,  (i32)(Offset + Position.x * CellSize), (i32)(Offset + Position.y * CellSize), WHITE);
        }
        Vector2 GenerateRandomCell()
        {
            f32 x = GetRandomValue(0, CellCount - 1);
            f32 y = GetRandomValue(0, CellCount - 1);
            return Vector2 {x, y};
        }
        Vector2 GenerateRandomPosition(deque<Vector2> SnakeBody)
        {
            Vector2 Position = GenerateRandomCell();
            while (ElementInDeque(Position, SnakeBody))
            {
                Position = GenerateRandomCell();
            }
            return Position;
        }
};
class Game {
    public:
        Snake snake;
        Food food = Food(snake.Body);
        bool Running = true;
        i32 Score = 0;
        Sound EatSound;
        Sound WallSound;

        Game()
        {
            InitAudioDevice();
            EatSound = LoadSound("../Sounds/eat.wav");
            WallSound = LoadSound("../Sounds/wall.wav");
        }
        ~Game()
        {
            UnloadSound(EatSound);
            UnloadSound(WallSound);
            CloseAudioDevice();
        }
        void Draw()
        {
            food.Draw();
            snake.Draw();
        }
        void Update()
        {
            if (Running)
            {
                snake.Update();
                CheckCollisionWithFood();
                CheckCollisionWithEdges();
                CheckCollisionWithTail();
            }
        }
        void CheckCollisionWithFood()
        {
            if (Vector2Equals(snake.Body[0], food.Position))
            {
                food.Position = food.GenerateRandomPosition(snake.Body);
                snake.AddSegment = true;
                Score++;
                PlaySound(EatSound);
            }
        }
        void CheckCollisionWithEdges()
        {
            if (snake.Body[0].x == CellCount || snake.Body[0].x == -1)
            {
                GameOver();
            }
            if (snake.Body[0].y == CellCount || snake.Body[0].y == -1)
            {
                GameOver();
            }
        }
        void CheckCollisionWithTail()
        {
            deque<Vector2> HeadlessBody = snake.Body;
            HeadlessBody.pop_front();
            if (ElementInDeque(snake.Body[0], HeadlessBody))
            {
                GameOver();
                std::cout << "Touched Tail" << std::endl;
            }
        }
        void GameOver()
        {
            snake.Reset();
            food.Position = food.GenerateRandomPosition(snake.Body);
            Running = false;
            Score = 0;
            PlaySound(WallSound);
        }
};

int main()
{
    const i32 FPS = 60;

    InitWindow((2 * Offset) + (CellSize * CellCount), (2 * Offset) + (CellSize * CellCount), "Snake");
    SetTargetFPS(FPS);

    Game Game;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        if (EventTriggered(0.2))
        {
            Game.Update();
        }
        if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && Game.snake.Direction.y != 1)
        {
            Game.snake.Direction = {0, -1};
            Game.Running = true;
        }
        if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && Game.snake.Direction.y != -1)
        {
            Game.snake.Direction = {0, 1};
            Game.Running = true;
        }
        if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && Game.snake.Direction.x != 1)
        {
            Game.snake.Direction = {-1, 0};
            Game.Running = true;
        }
        if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && Game.snake.Direction.x != -1)
        {
            Game.snake.Direction = {1, 0};
            Game.Running = true;
        }

        ClearBackground(MoonStone);
        Rectangle Rect = Rectangle {(f32)Offset - 5, (f32)Offset - 5, (f32)CellSize * CellCount + 10, (f32)CellSize * CellCount + 10};
        DrawRectangleLinesEx(Rect, 5, DarkSlateGray);
        DrawText("Snake", Offset - 5, 20, 40, DarkSlateGray);
        DrawText(TextFormat("Score: %i" ,Game.Score), Offset - 5, (Offset + CellSize * CellCount + 10), 40, DarkSlateGray);
        Game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
