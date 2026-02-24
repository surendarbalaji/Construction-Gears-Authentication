#include "raylib.h"
#include "constructions/constructions.h"
#include "gear.h"

int main(void) {
    const int screenWidth = 1600;
    const int screenHeight = 900;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Construction Gears Authentication");

    ColourConfiguration BluePrint = {.line1 = RAYWHITE, .arc1 = BLUE, .alpha = 1.0f};
    ColourConfiguration MonoChromed = {.line1 = RAYWHITE, .arc1 = GRAY, .alpha = 1.0f};
    ColourConfiguration PoliceConstructions = {.line1 = RED, .arc1 = BLUE, .alpha = 1.0f};
    ColourConfiguration FriedEggInversion = {.line1 = RAYWHITE, .arc1 = ORANGE, .alpha = 1.0f};
    ColourConfiguration SunnySideUp = {.line1 = ORANGE, .arc1 = WHITE, .alpha = 1.0f};

    // select universal colour theme here
    ColourConfiguration SelectedColours = PoliceConstructions;

    // creating the centered equilateral
    // Vector2 point1 = {screenWidth/2, (screenHeight/2) - 200};
    // Vector2 point2 = {(screenWidth/2) - 200 * cosf(PI/6), (screenHeight/2) + (200 * sinf(PI/6))};
    // Vector2 point3 = {(screenWidth/2) + 200 * cosf(PI/6), (screenHeight/2) + (200 * sinf(PI/6))};

    // first test series of reverse constructions
    // Vector2 perp1_input1 = { point2.x - 200 * cosf(5*PI/6), point2.y + 200 * sinf(5*PI/6) };
    // Vector2 perp1_input2 = { point2.x + 200 * cosf(5*PI/6), point2.y - 200 * sinf(5*PI/6) };
    // PerpendicularConstructor perpendicular1 = InitialisePerpendicular(perp1_input1, perp1_input2, point2, 0.5f, 1.0f, SelectedColours);

    Gear gear;
    gear.instruction_count = 0;
    gear.active_instruction = 0;
    gear.position = (Vector2){screenWidth/2, screenHeight/2};
    gear.currentPointA = gear.position;
    gear.currentPointB = (Vector2){screenWidth/2 + 100, screenHeight/2 + 100};
    gear.angle = 0.5f;
    gear.colours = SelectedColours;

    AddInstruction(&gear, CIRCLE);

    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        // UpdatePerpendicular(&perpendicular1, GetFrameTime());

        UpdateGear(&gear, GetFrameTime());


        BeginDrawing();

            ClearBackground(BLACK);

            DrawText("gear test", 190, 200, 20, LIGHTGRAY);


            // DrawLineV(point1, point2, WHITE);
            // DrawLineV(point1, point3, WHITE);
            // DrawLineV(point2, point3, WHITE);

            // DrawPerpendicularConstruction(&perpendicular1);

            DrawGear(&gear);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}