#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"
#include "gear.h"
#include "constructions/constructions.h"


float speed = 1.0f;
float thickness = 1.0f;

float fadeRate = 0.4f;
float fadeMin = 0.4f;

void AddInstruction(Gear* gear, ConstructionType type) {

    if (gear->instruction_count >= 32) return;

    Construction* construction = &gear->instructions[gear->instruction_count];
    construction->type = type;
    gear->instruction_count++;

    switch(type) {

        case CIRCLE:
            printf("circle added\n");
            printf("instruction count: %i\n", gear->instruction_count);
            construction->data.circle = InitialiseCircle(gear->currentPointA, gear->currentPointB, speed, thickness, gear->colours);
            break;

        case EQUILATERAL:
                printf("equilateral added\n");
                printf("instruction count: %i\n", gear->instruction_count);
                construction->data.equilateral = InitialiseEquilateral(gear->currentPointA, gear->currentPointB, (Vector2){gear->currentPointA.x - 50.0f, gear->currentPointA.y - 50.0f}, speed, thickness, gear->colours);
                break;

        case PERPENDICULAR:
            printf("perpendicular added\n");
            printf("instruction count: %i\n", gear->instruction_count);
            construction->data.perpendicular = InitialisePerpendicular(gear->currentPointA, gear->currentPointB, Vector2Lerp(gear->currentPointA, gear->currentPointB, 0.5f), speed, thickness, gear->colours);
            break;

        case PERPENDICULAR_BISECTOR:
            printf("perpendicular bisector added\n");
            printf("instruction count: %i\n", gear->instruction_count);
            construction->data.perpendicularBisector = InitialiseBisector(gear->currentPointA, gear->currentPointB, speed + 2.0f, thickness, gear->colours);
            break;
    }
}

void UpdateGear(Gear* gear, float dt) {

    if (gear->active_instruction >= gear->instruction_count) return;

    Construction* current_construction = &gear->instructions[gear->active_instruction];

    bool completed = false;

    switch(current_construction->type) {

        case CIRCLE:
            UpdateCircle(&current_construction->data.circle, dt);
            if (current_construction->data.circle.complete) completed = true;
            break;

        case EQUILATERAL:
            UpdateEquilateral(&current_construction->data.equilateral, dt);
            if (current_construction->data.equilateral.phase == EQUILATERALCOMPLETE) completed = true;
            break;

        case PERPENDICULAR:
            UpdatePerpendicular(&current_construction->data.perpendicular, dt);
            if (current_construction->data.perpendicular.phase == PERPENDICULARCOMPLETE) completed = true;
            break;

        case PERPENDICULAR_BISECTOR:
            UpdateBisector(&current_construction->data.perpendicularBisector, dt);
            if (current_construction->data.perpendicularBisector.phase == BISECTORCOMPLETE) completed = true;
            break;
    }

    for (int i = 0; i < gear->active_instruction; i++) {
        Construction* old_construction = &gear->instructions[i];

        switch(old_construction->type) {

            case CIRCLE:
                FadeConstruction(&old_construction->data.circle.colours, fadeRate, fadeMin, dt);
                break;

            case EQUILATERAL:
                FadeConstruction(&old_construction->data.equilateral.colours, fadeRate, fadeMin, dt);
                FadeConstruction(&old_construction->data.equilateral.circleA.colours, fadeRate, fadeMin, dt);
                FadeConstruction(&old_construction->data.equilateral.circleB.colours, fadeRate, fadeMin, dt);
                break;

            case PERPENDICULAR:
                FadeConstruction(&old_construction->data.perpendicular.colours, fadeRate, fadeMin, dt);
                FadeConstruction(&old_construction->data.perpendicular.circleAC.colours, fadeRate, fadeMin, dt);

                FadeConstruction(&old_construction->data.perpendicular.equilateralED.colours, fadeRate, fadeMin, dt);
                FadeConstruction(&old_construction->data.perpendicular.equilateralED.circleA.colours, fadeRate, fadeMin, dt);
                FadeConstruction(&old_construction->data.perpendicular.equilateralED.circleB.colours, fadeRate, fadeMin, dt);

                break;

            case PERPENDICULAR_BISECTOR:
                FadeConstruction(&old_construction->data.perpendicularBisector.colours, fadeRate, fadeMin, dt);
                break;
        }
    }

    if (completed) {

        switch (current_construction->type) {

            case CIRCLE:
                gear->currentPointA = current_construction->data.circle.centre;
                float newAngle = current_construction->data.circle.initial_angle + current_construction->data.circle.progress * 2 * PI;
                gear->currentPointB = (Vector2){current_construction->data.circle.centre.x + current_construction->data.circle.radius * cosf(newAngle), current_construction->data.circle.centre.y + current_construction->data.circle.radius * sinf(newAngle)};
                current_construction->data.circle.hideMask = CIRCLE_HIDELINE;
                break;

            case EQUILATERAL:
                gear->currentPointA = current_construction->data.equilateral.pointA;
                gear->currentPointB = current_construction->data.equilateral.pointC;
                break;

            case PERPENDICULAR:
                gear->currentPointA = current_construction->data.perpendicular.pointD;
                gear->currentPointB = current_construction->data.perpendicular.pointE;
                break;

            case PERPENDICULAR_BISECTOR:
                gear->currentPointA = current_construction->data.perpendicularBisector.pointC;
                gear->currentPointB = current_construction->data.perpendicularBisector.pointD;
                break;
        }

        gear->active_instruction++;

    }
}

void DrawGear(Gear* gear) {

    DrawCircleV(gear->position, 3.0f, WHITE);


    for (int i = 0; i < gear->instruction_count && i <= gear->active_instruction; i++) {
        Construction* construction = &gear->instructions[i];

        switch(construction->type) {

            case CIRCLE:
                DrawCircleConstruction(&construction->data.circle);
                break;

            case EQUILATERAL:
                DrawEquilateralConstruction(&construction->data.equilateral);
                break;

            case PERPENDICULAR:
                DrawPerpendicularConstruction(&construction->data.perpendicular);
                break;

            case PERPENDICULAR_BISECTOR:
                DrawBisectorConstruction(&construction->data.perpendicularBisector);
                break;

        }
    }
}