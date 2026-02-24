#include "constructions.h"
#include "raymath.h"

ColourConfiguration perpendicularColours = { .line1 = WHITE, .arc1 = BLUE};


PerpendicularConstructor InitialisePerpendicular(Vector2 pointA, Vector2 pointB, Vector2 pointC, float speed, float thickness, ColourConfiguration SelectedColours) {
    PerpendicularConstructor perpendicular;
    perpendicular.pointA = pointA;
    perpendicular.pointB = pointB;
    perpendicular.pointC = pointC;
    perpendicular.length = Vector2Distance(pointA, pointB);
    perpendicular.speed = speed;
    perpendicular.thickness = thickness;
    perpendicular.colours = SelectedColours;
    perpendicular.angle_AB = atan2f(pointB.y - pointA.y, pointB.x - pointA.x);
    perpendicular.progress = 0.0f;
    perpendicular.phase = CIRCLEAC;

    perpendicular.pointE = (Vector2){pointC.x + (perpendicular.length / 2) * cosf(perpendicular.angle_AB), pointC.y + (perpendicular.length / 2) * sinf(perpendicular.angle_AB)};
    perpendicular.pointD = (Vector2){pointC.x - (perpendicular.length / 2) * cosf(perpendicular.angle_AB), pointC.y - (perpendicular.length / 2) * sinf(perpendicular.angle_AB)};

    perpendicular.circleAC = InitialiseCircle(pointA, (Vector2){0.5 * (pointA.x + pointB.x), 0.5 * (pointA.y + pointB.y)}, speed, thickness, perpendicular.colours);
    perpendicular.equilateralED = InitialiseEquilateral(perpendicular.pointE, perpendicular.pointD, pointC, speed, thickness, SelectedColours);

    return perpendicular;
}


void UpdatePerpendicular(PerpendicularConstructor* perpendicular, float dt) {
    if (perpendicular->phase == PERPENDICULARCOMPLETE) return;

    switch (perpendicular->phase) {

        case CIRCLEAC:
            UpdateCircle(&perpendicular->circleAC, dt);
            if (perpendicular->circleAC.complete) perpendicular->phase = EQUILATERALDE;
            break;

        case EQUILATERALDE:
            perpendicular->circleAC.hideMask = CIRCLE_HIDELINE;
            UpdateEquilateral(&perpendicular->equilateralED, dt);
            if (perpendicular->equilateralED.phase == EQUILATERALCOMPLETE) perpendicular->phase = LINEFC;
            break;

        case LINEFC:
            perpendicular->progress += perpendicular->speed * dt;
            if (perpendicular->progress >= 1.0f) {
                perpendicular->progress = 0.0f;
                perpendicular->phase = PERPENDICULARCOMPLETE;
            }
            break;

        case PERPENDICULARCOMPLETE:
            perpendicular->phase = PERPENDICULARCOMPLETE;
            break;
    }
}

void DrawPerpendicularConstruction(const PerpendicularConstructor* perpendicular) {

    switch (perpendicular->phase) {

        case CIRCLEAC:
            DrawCircleConstruction(&perpendicular->circleAC);
            break;

        case EQUILATERALDE:
            DrawCircleConstruction(&perpendicular->circleAC);
            DrawEquilateralConstruction(&perpendicular->equilateralED);
        break;

        case LINEFC:
            DrawCircleConstruction(&perpendicular->circleAC);
            DrawEquilateralConstruction(&perpendicular->equilateralED);

            DrawLineEx(perpendicular->equilateralED.pointC, Vector2Lerp(perpendicular->equilateralED.pointC, perpendicular->pointC, perpendicular->progress), perpendicular->thickness, perpendicular->colours.line1);
            break;

        case PERPENDICULARCOMPLETE:
            DrawCircleConstruction(&perpendicular->circleAC);
            DrawEquilateralConstruction(&perpendicular->equilateralED);
            DrawLineEx(perpendicular->equilateralED.pointC, perpendicular->pointC, perpendicular->thickness, perpendicular->colours.line1);
            break;
    }
}

