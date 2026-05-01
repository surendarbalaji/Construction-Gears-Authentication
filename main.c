#include "raylib.h"
#include "constructions/constructions.h"
#include "gear.h"
#include "input.h"

typedef enum {
    SETUP,
    PLACING,
    COLOURS,
    FREQUENCIES,
    CONSTRUCT
} ProgramState;

int constructionFrequencies[4] = {1, 1, 1, 1};
const char* themeName1 = "Mono";
const char* themeName2 = "Chromed";

int main(void) {
    const int screenWidth = 1600;
    const int screenHeight = 900;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Construction Gears");

    ProgramState state = SETUP;

    Gear gears[32] = { 0 };
    int gearCount = 1;
    int gearsPlaced = 0;

    Font creatoDisplay = LoadFontEx("../resources/CreatoDisplay-ExtraBold.ttf", 20.0f, 0, 0);

    ColourConfiguration MonoChromed = {.line1 = RAYWHITE, .arc1 = GRAY, .alpha = 1.0f};
    ColourConfiguration BluePrint = {.line1 = RAYWHITE, .arc1 = BLUE, .alpha = 1.0f};
    ColourConfiguration PoliceConstructions = {.line1 = RED, .arc1 = BLUE, .alpha = 1.0f};
    ColourConfiguration FriedEggInversion = {.line1 = RAYWHITE, .arc1 = ORANGE, .alpha = 1.0f};
    ColourConfiguration SunnySideUp = {.line1 = ORANGE, .arc1 = WHITE, .alpha = 1.0f};


    // select universal colour theme here
    ColourConfiguration SelectedColours = MonoChromed;

    // creating the centered equilateral
    // Vector2 point1 = {screenWidth/2, (screenHeight/2) - 200};
    // Vector2 point2 = {(screenWidth/2) - 200 * cosf(PI/6), (screenHeight/2) + (200 * sinf(PI/6))};
    // Vector2 point3 = {(screenWidth/2) + 200 * cosf(PI/6), (screenHeight/2) + (200 * sinf(PI/6))};

    // Gear gear = { 0 };
    // gear.number = 1;
    // gear.position = (Vector2){screenWidth/2.0f, screenHeight/2.0f};
    // gear.currentPointA = gear.position;
    // gear.currentPointB = (Vector2){gear.position.x + 140.0f, gear.position.y};
    // gear.colours = SelectedColours;
    //
    // Gear gear2 = { 0 };
    // gear2.number = 2;
    // gear2.position = (Vector2){screenWidth/2.0f + 450.0f, screenHeight/2.0f - 60.0f};
    // gear2.currentPointA = gear2.position;
    // gear2.currentPointB = (Vector2){gear2.position.x + 140.0f, gear2.position.y - 20.0f};
    // gear2.colours = SelectedColours;
    //
    // Gear gear3 = { 0 };
    // gear3.number = 3;
    // gear3.position = (Vector2){screenWidth/2.0f - 400.0f, screenHeight/2.0f + 100.0f};
    // gear3.currentPointA = gear3.position;
    // gear3.currentPointB = (Vector2){gear3.position.x - 140.0f, gear3.position.y + 80.0f};
    // gear3.colours = SelectedColours;


    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        switch (state) {

            case SETUP:

                if ((IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP)) && gearCount < 32) gearCount++;
                if ((IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN)) && gearCount > 1) gearCount--;

                if (IsKeyPressed(KEY_ENTER)) state = PLACING;

                break;

            case PLACING:

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

                    // gears[gearsPlaced] = { 0 };

                    gears[gearsPlaced].number = gearsPlaced + 1;
                    gears[gearsPlaced].position = GetMousePosition();
                    gears[gearsPlaced].currentPointA = GetMousePosition();

                    gears[gearsPlaced].currentPointB = (Vector2){GetMousePosition().x + 100.0f, GetMousePosition().y};

                    gearsPlaced++;

                }

                if (gearsPlaced == gearCount) state = COLOURS;

                break;

            case COLOURS:

                if(IsKeyPressed(KEY_ONE)) {
                    SelectedColours = MonoChromed;
                    themeName1 = "Mono";
                    themeName2 = "Chromed";
                }

                if(IsKeyPressed(KEY_TWO)) {
                    SelectedColours = BluePrint;
                    themeName1 = "Blue";
                    themeName2 = "Print";
                }

                if(IsKeyPressed(KEY_THREE)) {
                    SelectedColours = PoliceConstructions;
                    themeName1 = "Police";
                    themeName2 = "Constructions";
                }

                if(IsKeyPressed(KEY_FOUR)) {
                    SelectedColours = FriedEggInversion;
                    themeName1 = "FriedEgg";
                    themeName2 = "Inversion";
                }

                if(IsKeyPressed(KEY_FIVE)) {
                    SelectedColours = SunnySideUp;
                    themeName1 = "SunnySide";
                    themeName2 = "Up";
                }

                if (IsKeyPressed(KEY_ENTER)) {
                    for (int i = 0; i < gearsPlaced; i++) gears[i].colours = SelectedColours;
                    state = CONSTRUCT;
                }

                break;

            case CONSTRUCT:

                takeInput();

                for (int i = 0; i < gearCount; i++) {
                    processInput(&gears[i]);
                    UpdateGear(&gears[i], GetFrameTime());
                }

                if (IsKeyDown(KEY_R) && (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))) {
                    for (int i = 0; i <= gearCount; i++) gears[i] = (Gear){0};
                    input[0] = '\0';
                    gearsPlaced = 0;
                    gearCount = 1;
                    letterCount = 0;
                    state = SETUP;
                }

                break;

        }

        BeginDrawing();

            ClearBackground(BLACK);

            switch (state) {

                case SETUP:

                    DrawTextEx(creatoDisplay, TextFormat("Choose number of gears (1-32): %d", gearCount), (Vector2){150, 150}, 20.0f, 1.0f, LIGHTGRAY);
                    DrawTextEx(creatoDisplay, "UP Arrow/Down Arrow to change, ENTER to confirm", (Vector2){150, 170}, 20.0f, 1.0f, LIGHTGRAY);
                    break;

                case PLACING:

                    DrawTextEx(creatoDisplay, TextFormat("Click to place gear %d/%d", gearsPlaced + 1, gearCount), (Vector2){150, 150}, 20.0f, 1.0f, LIGHTGRAY);
                    DrawCircleV(GetMousePosition(), 5.0f, LIGHTGRAY);

                    for (int i = 0; i < gearsPlaced; i++) DrawCircleV(gears[i].position, 5.0f, LIGHTGRAY);

                    break;

                case COLOURS:

                    DrawTextEx(creatoDisplay, "Select colour theme (NUM Keys 1-5): ", (Vector2){150, 150}, 20.0f, 1.0f, LIGHTGRAY);
                    DrawTextEx(creatoDisplay, themeName1, (Vector2){460, 150}, 20.0f, 1.0f, SelectedColours.line1);
                    DrawTextEx(creatoDisplay, themeName2, (Vector2){460.0f + MeasureTextEx(creatoDisplay, themeName1, 20.0f, 1.0f).x + 2.0f, 150}, 20.0f, 1.0f, SelectedColours.arc1);

                    for (int i = 0; i < gearCount; i++) DrawCircleV(gears[i].position, 5.0f, LIGHTGRAY);

                    break;

                case CONSTRUCT:

                    DrawTextEx(creatoDisplay, input, (Vector2){150, 150}, 20.0f, 1.0f, LIGHTGRAY);

                    for (int i = 0; i < gearCount; i++) {

                        DrawGear(&gears[i]);

                        if (i == gearCount - 1) DrawTextEx(creatoDisplay, TextFormat("Gear %i: %i / %i instructions", gears[i].number, gears[i].active_instruction, letterCount), (Vector2){150, 180.0f + i * 20.0f}, 20.0f, 1.0f, GRAY);

                        DrawTextEx(creatoDisplay, TextFormat("Gear %i: %i", gears[i].number, gears[i].active_instruction), (Vector2){150, 180.0f + i * 20.0f}, 20.0f, 1.0f, GRAY);

                    }

                    DrawTextEx(creatoDisplay, "SHIFT + R to Restart", (Vector2){150, 200.0f + gearCount * 20.0f}, 20.0f, 1.0f, DARKGRAY);

                    break;

            }

            // DrawText("gear test", 190, 200, 20, LIGHTGRAY);

            // DrawLineV(point1, point2, WHITE);
            // DrawLineV(point1, point3, WHITE);
            // DrawLineV(point2, point3, WHITE);

            // BeginBlendMode(BLEND_ADDITIVE);
            // DrawGear(&gear);
            // DrawGear(&gear2);
            // DrawGear(&gear3);
            // BeginBlendMode(BLEND_ALPHA);

            // DrawText(input, 150, 150, 20, LIGHTGRAY);

            // DrawTextEx(creatoDisplay, TextFormat("Gear 1: %i/%i instructions", gear.active_instruction, letterCount), (Vector2){150, 180}, 20.0f, 1.0f, GRAY);
            // DrawTextEx(creatoDisplay, TextFormat("Gear 2: %i/%i instructions", gear2.active_instruction, letterCount), (Vector2){150, 200}, 20.0f, 1.0f, GRAY);
            // DrawTextEx(creatoDisplay, TextFormat("Gear 3: %i/%i instructions", gear3.active_instruction, letterCount), (Vector2){150, 220}, 20.0f, 1.0f, GRAY);

        EndDrawing();
    }

    UnloadFont(creatoDisplay);
    CloseWindow();

    return 0;
}