#include "raylib.h"
#include "constructions.h"


int main(void) {
    const int screenWidth = 1600;
    const int screenHeight = 900;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Construction Gears Authentication");

    Vector2 v0 = { screenWidth/2.0f, screenHeight/2.0f };
    Vector2 v1 = { screenWidth/2.0f - 60.0f, screenHeight/2.0f + 120.0f };
    Vector2 v2 = { 0 };
    Vector2 v3 = { screenWidth/2.0f + 320.0f, screenHeight/2.0f + 160.0f  };
    Vector2 v4 = { screenWidth/2.0f + 160.0f, screenHeight/2.0f + 260.0f  };
    Vector2 v5 = {screenWidth/2.0f + 150.0f, screenHeight/2.0f -300.0f };
    Vector2 v6 = {screenWidth/2.0f + 200.0f, screenHeight/2.0f +100.0f };

    // CircleConstructor circle = InitialiseCircle(v3, v4, 2.0f, 2.0f, GOLD, GRAY);
    // BisectorConstructor bisector = InitialiseBisector(v5, v6, 2.5f, 2.0f, BLUE, RED);
    // EquilateralConstructor equilateral = InitialiseEquilateral(v0, v1, 3.5f, 2.0f, ORANGE, RAYWHITE );
    EqualLineConstructor equalLine = InitialiseEqualLine(v0, v1, v6, 3.5f, 2.0f, ORANGE, RAYWHITE);

    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        v2 = GetMousePosition();

        // printf("(%f, %f)\n ", v2.x, v2.y);
        // float initial_angle = atan2f(v2.y - v0.y, v2.x - v0.x);
        // float bisector_angle = initial_angle + PI/2;
        // printf("%f, %f\n", initial_angle, bisector_angle);

        // UpdateCircle(&circle, GetFrameTime());
        // UpdateBisector(&bisector, GetFrameTime());
        // UpdateEquilateral(&equilateral, GetFrameTime());
        UpdateEqualLine(&equalLine, GetFrameTime());


        BeginDrawing();

            ClearBackground(BLACK);

            DrawText("the elements", 190, 200, 20, LIGHTGRAY);

            // BeginBlendMode(1);

            // DrawRing(v0, 200.0f, 205.0f, 0, 180, 100, RAYWHITE);

            // DrawLine(v0.x, v0.y, v2.x, v2.y, BLACK);
            // DrawLineV(v0, v2, BLACK);
            // DrawLineEx(v0, v2, 2.0f, ORANGE);
            // DrawCircleLinesV(v0, 5, ORANGE);

            // DrawCircleConstruction(&circle);
            // DrawBisectorConstruction(&bisector);
            // DrawEquilateralConstruction(&equilateral);
            DrawEqualLineConstruction(&equalLine);

        // EndBlendMode();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}