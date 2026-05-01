#include "constructions.h"
#include "raymath.h"

ColourConfiguration equilateralColours = { .line1 = WHITE, .arc1 = BLUE};


EquilateralConstructor InitialiseEquilateral(Vector2 pointA, Vector2 pointB, Vector2 faceAwayFrom, float speed, float thickness, ColourConfiguration SelectedColours) {
    EquilateralConstructor equilateral;
    equilateral.pointA = pointA;
    equilateral.pointB = pointB;
    equilateral.faceAwayFrom = faceAwayFrom;
    equilateral.length = Vector2Distance(pointA, pointB);
    equilateral.speed = speed;
    equilateral.thickness = thickness;
    equilateral.colours = SelectedColours;
    equilateral.initial_angle = atan2f(pointB.y - pointA.y, pointB.x - pointA.x);

    // finding both intersections to allow for selection in certain situations (like wanting the triangle to face away from an angle bisection)

    Vector2 point1 = (Vector2) {
        pointA.x + equilateral.length * cosf(equilateral.initial_angle + PI/3),
        pointA.y + equilateral.length * sinf(equilateral.initial_angle + PI/3)}; // intersection point 1

    Vector2 point2 = (Vector2) {
        pointA.x + equilateral.length * cosf(equilateral.initial_angle - PI/3),
        pointA.y + equilateral.length * sinf(equilateral.initial_angle - PI/3)}; // intersection point 2

    equilateral.pointC = (Vector2Distance(point1, faceAwayFrom) > Vector2Distance(point2, faceAwayFrom)) ? point1 : point2;

    equilateral.progress = 0.0f;
    equilateral.phase = CIRCLES;
    equilateral.hideMask = 0;

    // assigning circle constructions to the equilateral construction
    equilateral.circleA = InitialiseCircle(pointA, pointB, speed, thickness, equilateral.colours);
    equilateral.circleB = InitialiseCircle(pointB, pointA, speed, thickness, equilateral.colours);

    equilateral.circleA.hideMask = CIRCLE_HIDELINE;
    equilateral.circleB.hideMask = CIRCLE_HIDELINE;

    return equilateral;
}


void UpdateEquilateral(EquilateralConstructor* equilateral, float dt) {
    switch (equilateral->phase) {

        case CIRCLES:
            UpdateCircle(&equilateral->circleA, dt);
            UpdateCircle(&equilateral->circleB, dt);
            if (equilateral->circleA.complete && equilateral->circleB.complete) equilateral->phase = SIDES;
            break;

        case SIDES:
            equilateral->progress += equilateral->speed * dt;
            if (equilateral->progress >= 1.0f) {
                equilateral->progress = 0.0f;
                equilateral->phase = EQUILATERALCOMPLETE;
            }
            break;

        case EQUILATERALCOMPLETE:
            equilateral->circleA.hideMask = CIRCLE_HIDELINE;
            equilateral->circleB.hideMask = CIRCLE_HIDELINE;
            FadeConstruction(&equilateral->circleA.colours, 0.1f, 0.2f, dt);
            FadeConstruction(&equilateral->circleB.colours, 0.1f, 0.2f, dt);
            FadeConstruction(&equilateral->colours, 0.1f, 0.2f, dt);
    }
}

void DrawEquilateralConstruction(const EquilateralConstructor* equilateral) {

    switch (equilateral->phase) {
        case CIRCLES:

            DrawCircleConstruction(&equilateral->circleA);
            DrawCircleConstruction(&equilateral->circleB);
            break;

        case SIDES:

            DrawCircleConstruction(&equilateral->circleA);
            DrawCircleConstruction(&equilateral->circleB);
            DrawLineEx(equilateral->pointA, equilateral->pointB, equilateral->thickness, equilateral->colours.line1);

            // using lerp to animate drawing, dividing by 2pi since progress is multiplied to be an angle
            DrawLineEx(equilateral->pointA, Vector2Lerp(equilateral->pointA, equilateral->pointC, equilateral->progress), equilateral->thickness, equilateral->colours.line1);
            DrawLineEx(equilateral->pointB, Vector2Lerp(equilateral->pointB, equilateral->pointC, equilateral->progress), equilateral->thickness, equilateral->colours.line1);
            break;

        case EQUILATERALCOMPLETE:

            DrawCircleConstruction(&equilateral->circleA);
            DrawCircleConstruction(&equilateral->circleB);

            if (!(equilateral->hideMask & EQUILATERAL_HIDEAB))
                DrawLineEx(equilateral->pointA, equilateral->pointB, equilateral->thickness, Fade(equilateral->colours.line1, equilateral->colours.alpha));
            if (!(equilateral->hideMask & EQUILATERAL_HIDEAC))
                DrawLineEx(equilateral->pointA, equilateral->pointC, equilateral->thickness, Fade(equilateral->colours.line1, equilateral->colours.alpha));
            if (!(equilateral->hideMask & EQUILATERAL_HIDEBC))
                DrawLineEx(equilateral->pointB, equilateral->pointC, equilateral->thickness, Fade(equilateral->colours.line1, equilateral->colours.alpha));

            break;
    }
}

