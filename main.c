#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "constructions.h"

int main(void) {

    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Construction Gears Authentication");

    float circleAngle = 0.0f;

    Vector2 v0 = { screenWidth/2.0f, screenHeight/2.0f };
    Vector2 v1 = { screenWidth/2.0f + 20.0f, screenHeight/2.0f + 20.0f };
    Vector2 v2 = { 0 };

    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        v2 = GetMousePosition();

        printf("(%f, %f)\n ", v2.x, v2.y);

        BeginDrawing();

            ClearBackground(BLACK);

            DrawText("The first window", 190, 200, 20, LIGHTGRAY);

            // DrawLine(v0.x, v0.y, v2.x, v2.y, BLACK);
            // DrawLineV(v0, v2, BLACK);
            DrawLineEx(v0, v2, 2.0f, ORANGE);

            // DrawCircleLinesV(v0, 5, ORANGE);
            ConstructCircle(v0, 50, &circleAngle, 6.0f, 2.0f, ORANGE, ORANGE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
