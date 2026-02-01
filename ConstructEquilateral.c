#include "constructions.h"
#include "raymath.h"

EquilateralConstructor InitialiseEquilateral(Vector2 pointA, Vector2 pointB, float speed, float thickness, Color arcColour, Color lineColour) {
    EquilateralConstructor equilateral;
    equilateral.pointA = pointA;
    equilateral.pointB = pointB;
    equilateral.length = Vector2Distance(pointA, pointB);
    equilateral.speed = speed;
    equilateral.thickness = thickness;
    equilateral.arcColour = arcColour;
    equilateral.lineColour = lineColour;
    equilateral.initial_angle = atan2f(pointB.y - pointA.y, pointB.x - pointA.x);
    equilateral.pointC = (Vector2){
        pointA.x + equilateral.length * cosf(equilateral.initial_angle + PI/3),
        pointA.y + equilateral.length * sinf(equilateral.initial_angle + PI/3)}; // intersection point
    equilateral.progress = 0.0f;
    equilateral.phase = CIRCLES;

    // assigning circle constructions to the equilateral construction
    equilateral.circleA = InitialiseCircle(pointA, pointB, speed, thickness, arcColour, lineColour);
    equilateral.circleB = InitialiseCircle(pointB, pointA, speed, thickness, arcColour, lineColour);

    return equilateral;
}


void UpdateEquilateral(EquilateralConstructor* equilateral, float dt) {

    equilateral->progress += equilateral->speed * dt;
    UpdateCircle(&equilateral->circleA, dt);
    UpdateCircle(&equilateral->circleB, dt);

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

            DrawCircleConstruction(&equilateral->circleA);
            DrawCircleConstruction(&equilateral->circleB);
            break;

        case SIDES:

            DrawCircleLinesV(equilateral->pointA, equilateral->length, equilateral->arcColour);
            DrawCircleLinesV(equilateral->pointB, equilateral->length, equilateral->arcColour);
            DrawLineEx(equilateral->pointA, equilateral->pointB, equilateral->thickness, equilateral->lineColour);

            // using lerp to animate drawing, dividing by 2pi since progress is multiplied to be an angle
            DrawLineEx(equilateral->pointA, Vector2Lerp(equilateral->pointA, equilateral->pointC, equilateral->progress / (2.0f * PI)), equilateral->thickness, equilateral->lineColour);
            DrawLineEx(equilateral->pointB, Vector2Lerp(equilateral->pointB, equilateral->pointC, equilateral->progress / (2.0f * PI)), equilateral->thickness, equilateral->lineColour);
            break;

        case EQUILATERALCOMPLETE:

            DrawCircleLinesV(equilateral->pointA, equilateral->length, equilateral->arcColour);
            DrawCircleLinesV(equilateral->pointB, equilateral->length, equilateral->arcColour);
            DrawLineEx(equilateral->pointA, equilateral->pointB, equilateral->thickness, equilateral->lineColour);

            DrawLineEx(equilateral->pointA, equilateral->pointC, equilateral->thickness, equilateral->lineColour);
            DrawLineEx(equilateral->pointB, equilateral->pointC, equilateral->thickness, equilateral->lineColour);
            break;
    }
}

