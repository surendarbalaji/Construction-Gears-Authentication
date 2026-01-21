#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "constructions.h"


int main(void) {

    const int screenWidth = 1600;
    const int screenHeight = 900;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Construction Gears Authentication");

    Vector2 v0 = { screenWidth/2.0f, screenHeight/2.0f };
    Vector2 v1 = { screenWidth/2.0f - 60.0f, screenHeight/2.0f + 120.0f };
    Vector2 v2 = { 0 };
    Vector2 v3 = { screenWidth/2.0f + 120.0f, screenHeight/2.0f - 120.0f  };
    Vector2 v4 = { screenWidth/2.0f - 100.0f, screenHeight/2.0f + 220.0f  };
    Vector2 v5 = {screenWidth/2.0f + 150.0f, screenHeight/2.0f -300.0f };
    Vector2 v6 = {screenWidth/2.0f + 200.0f, screenHeight/2.0f +100.0f };

    // CircleConstructor circle = InitialiseCircle(v0, v1, 3.0f, 2.0f, BLUE, RED);
    // CircleConstructor circle2 = InitialiseCircle(v3, v4,3.0f, 2.0f, BLUE, RED);

    BisectorConstructor bisector = InitialiseBisector(v5, v6, 1.5f, 2.0f, BLUE, RED);

    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        v2 = GetMousePosition();

        // printf("(%f, %f)\n ", v2.x, v2.y);
        // float initial_angle = atan2f(v2.y - v0.y, v2.x - v0.x);
        // float bisector_angle = initial_angle + PI/2;
        // printf("%f, %f\n", initial_angle, bisector_angle);

        // UpdateCircle(&circle, GetFrameTime());
        // UpdateCircle(&circle2, GetFrameTime());
        UpdateBisector(&bisector, GetFrameTime());

        BeginDrawing();

            ClearBackground(BLACK);

            DrawText("perpendicular bisector construction", 190, 200, 20, LIGHTGRAY);

            // DrawLine(v0.x, v0.y, v2.x, v2.y, BLACK);
            // DrawLineV(v0, v2, BLACK);
            // DrawLineEx(v0, v2, 2.0f, ORANGE);

            // DrawCircleLinesV(v0, 5, ORANGE);
            // DrawCircleConstruction(&circle);
            // DrawCircleConstruction(&circle2);
            DrawBisectorConstruction(&bisector);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
