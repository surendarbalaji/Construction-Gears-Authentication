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

    ColourConfiguration BluePrint = {.line1 = RAYWHITE, .arc1 = BLUE, .alpha = 1.0f};
    ColourConfiguration MonoChromed = {.line1 = RAYWHITE, .arc1 = GRAY, .alpha = 1.0f};
    ColourConfiguration PoliceConstructions = {.line1 = RED, .arc1 = BLUE, .alpha = 1.0f};
    ColourConfiguration FriedEggInversion = {.line1 = RAYWHITE, .arc1 = ORANGE, .alpha = 1.0f};
    ColourConfiguration SunnySideUp = {.line1 = ORANGE, .arc1 = WHITE, .alpha = 1.0f};

    ColourConfiguration SelectedColours = BluePrint; // select universal colour theme here


    // CircleConstructor circle = InitialiseCircle(v3, v4, 2.0f, 0.5f, SelectedColours);
    // BisectorConstructor bisector = InitialiseBisector(v5, v6, 2.5f, 2.0f, SelectedColours);
    // EquilateralConstructor equilateral = InitialiseEquilateral(v0, v1, (Vector2){0}, 0.5f, 2.0f, SelectedColours);
    // EqualLineConstructor equalLine = InitialiseEqualLine(v0, v1, v6, 1.0f, 2.0f, SelectedColours);
    // AngleBisectorConstructor angleBisector = InitialiseAngleBisector(v1, v5, v6, 1.0f, 2.0f, SelectedColours);
    // EqualLengthCutConstructor equalLengthCut = InitialiseEqualLengthCut(v1, v3, v5, v6, 0.5f, 2.0f, SelectedColours);
    EqualAngleConstructor equalAngle = InitialiseEqualAngle(v5, v3, v4, v0, v1, 1.5f, 2.0f, SelectedColours);
    // ParallelConstructor parallel = InitialiseParallel(v5, v3, v0, 4.0f, 1.0f, SelectedColours);
    // PerpendicularConstructor perpendicular = InitialisePerpendicular(v0, v1, v0, 0.5f, 1.0f, SelectedColours);
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
        // UpdateEqualLine(&equalLine, GetFrameTime());
        // UpdateAngleBisector(&angleBisector, GetFrameTime());
        // UpdateEqualLengthCut(&equalLengthCut, GetFrameTime());
        UpdateEqualAngle(&equalAngle, GetFrameTime());
        // UpdateParallel(&parallel, GetFrameTime());
        // UpdatePerpendicular(&perpendicular, GetFrameTime());

        BeginDrawing();

            ClearBackground(BLACK);

            DrawText("equal angle construction", 190, 200, 20, LIGHTGRAY);

            // BeginBlendMode(1);

            // DrawRing(v0, 200.0f, 205.0f, 0, 180, 100, RAYWHITE);

            // DrawLine(v0.x, v0.y, v2.x, v2.y, BLACK);
            // DrawLineV(v0, v2, BLACK);
            // DrawLineEx(v0, v2, 2.0f, ORANGE);
            // DrawCircleLinesV(v0, 5, ORANGE);

            // DrawCircleConstruction(&circle);
            // DrawBisectorConstruction(&bisector);
            // DrawEquilateralConstruction(&equilateral);
            // DrawEqualLineConstruction(&equalLine);
            // DrawAngleBisectorConstruction(&angleBisector);
            // DrawEqualLengthCutConstruction(&equalLengthCut);
            DrawEqualAngleConstruction(&equalAngle);
            // DrawParallelConstruction(&parallel);
            // DrawPerpendicularConstruction(&perpendicular);

        // EndBlendMode();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}