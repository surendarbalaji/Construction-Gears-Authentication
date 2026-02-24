#include "gear.h"
#include "constructions/constructions.h"

#include <math.h>
#include <stdio.h>

float speed = 1.0f;
float thickness = 1.0f;

void AddInstruction(Gear* gear, ConstructionType type) {
    Construction* construction = &gear->instructions[gear->instruction_count];
    construction->type = type;
    gear->instruction_count++;

    switch(construction->type) {
        case CIRCLE:
            printf("circle added\n");
            printf("instruction count: %i\n", gear->instruction_count);
            construction->data.circle = InitialiseCircle(gear->currentPointA, gear->currentPointB, speed, thickness, gear->colours);
            break;

        case PERPENDICULAR:
            construction->data.perpendicularBisector = InitialiseBisector(gear->currentPointA, gear->currentPointB, speed, thickness, gear->colours.arc1, gear->colours.line1 );
            break;

        case PERPENDICULAR_BISECTOR:
            construction->data.perpendicular = InitialisePerpendicular(gear->currentPointA, gear->currentPointB, gear->currentPointA, speed, thickness, gear->colours);
            break;
    }
}

void UpdateGear(Gear* gear, float dt) {
    Construction* current_construction = &gear->instructions[gear->active_instruction];

    switch(current_construction->type) {

        case CIRCLE:
            printf("dt: %f\n", dt);
            printf("%i\n", gear->active_instruction);
            UpdateCircle(&current_construction->data.circle, dt);
            printf("%f\n", current_construction->data.circle.progress);
            gear->currentPointA = current_construction->data.circle.centre;
            gear->currentPointB = (Vector2){current_construction->data.circle.centre.x + current_construction->data.circle.radius * cosf(gear->angle), current_construction->data.circle.centre.y + current_construction->data.circle.radius * sinf(gear->angle)};
            if (current_construction->data.circle.complete) {
                // gear->active_instruction++;
            }
            break;

        case PERPENDICULAR:
            UpdatePerpendicular(&current_construction->data.perpendicular, dt);
            gear->currentPointA = current_construction->data.perpendicular.pointE;
            gear->currentPointB = current_construction->data.perpendicular.pointD;
            break;

        case PERPENDICULAR_BISECTOR:
            UpdateBisector(&current_construction->data.perpendicularBisector, dt);
            // leaving this for now because i dont want to go back and fix the code to calculate endpoints
            break;
    }
}

void DrawGear(Gear* gear) {

    DrawCircleV(gear->position, 3.0f, WHITE);

    for (int i = 0; i < gear->instruction_count; i++) {
        Construction* construction = &gear->instructions[i];

        switch(construction->type) {

            case CIRCLE:
                printf("drawing circle\n");
                DrawCircleConstruction(&construction->data.circle);
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