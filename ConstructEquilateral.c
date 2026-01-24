#include "constructions.h"
#include "raymath.h"
#include <stdio.h>

EquilateralConstructor InitialiseEquilateral(Vector2 point1, Vector2 point2, float speed, float thickness, Color arcColour, Color lineColour) {
    EquilateralConstructor equilateral;
    equilateral.point1 = point1;
    equilateral.point2 = point2;
    equilateral.length = Vector2Distance(point1, point2);
    equilateral.speed = speed;
    equilateral.thickness = thickness;
    equilateral.arcColour = arcColour;
    equilateral.lineColour = lineColour;
    equilateral.initial_angle = atan2f(point2.y - point1.y, point2.x - point1.x);
    equilateral.point3 = (Vector2){
        point1.x + equilateral.length * cosf(equilateral.initial_angle + PI/3),
        point1.y + equilateral.length * sinf(equilateral.initial_angle + PI/3)}; // intersection point
    equilateral.progress = 0.0f;
    equilateral.phase = CIRCLES;

    // assigning circle constructions to the equilateral construction
    equilateral.circleA = InitialiseCircle(point1, point2, speed, thickness, arcColour, lineColour);
    equilateral.circleB = InitialiseCircle(point2, point1, speed, thickness, arcColour, lineColour);

    return equilateral;
}


void UpdateEquilateral(EquilateralConstructor* equilateral, float dt) {

    equilateral->progress += equilateral->speed * dt;
    UpdateCircle(&equilateral->circleA, GetFrameTime());
    UpdateCircle(&equilateral->circleB, GetFrameTime());

    if (equilateral->progress >= 2.0f * PI) {
        equilateral->progress = 0.0f;

        switch (equilateral->phase) {
            case CIRCLES:
                equilateral->phase = SIDES;
                break;
            case SIDES:
                equilateral->phase = EQUILATERALCOMPLETE;
                break;
            case EQUILATERALCOMPLETE:
                equilateral->phase = EQUILATERALCOMPLETE;
                break;
        }

    }
}

void DrawEquilateralConstruction(const EquilateralConstructor* equilateral) {

    switch (equilateral->phase) {
        case CIRCLES:
            printf("Phase circles\n");
            DrawCircleConstruction(&equilateral->circleA);
            DrawCircleConstruction(&equilateral->circleB);
            break;

        case SIDES:
            printf("Phase sides\n");
            DrawCircleLinesV(equilateral->point1, equilateral->length, equilateral->arcColour);
            DrawCircleLinesV(equilateral->point2, equilateral->length, equilateral->arcColour);
            DrawLineEx(equilateral->point1, equilateral->point2, equilateral->thickness, equilateral->lineColour);

            // using lerp to animate drawing, dividing by 2pi since progress is multiplied to be an angle
            DrawLineEx(equilateral->point1, Vector2Lerp(equilateral->point1, equilateral->point3, equilateral->progress / (2.0f * PI)), equilateral->thickness, equilateral->lineColour);
            DrawLineEx(equilateral->point2, Vector2Lerp(equilateral->point2, equilateral->point3, equilateral->progress / (2.0f * PI)), equilateral->thickness, equilateral->lineColour);
            break;

        case EQUILATERALCOMPLETE:
            printf("Phase complete\n");
            DrawCircleLinesV(equilateral->point1, equilateral->length, equilateral->arcColour);
            DrawCircleLinesV(equilateral->point2, equilateral->length, equilateral->arcColour);
            DrawLineEx(equilateral->point1, equilateral->point2, equilateral->thickness, equilateral->lineColour);

            DrawLineEx(equilateral->point1, equilateral->point3, equilateral->thickness, equilateral->lineColour);
            DrawLineEx(equilateral->point2, equilateral->point3, equilateral->thickness, equilateral->lineColour);
            break;
    }
}