#include "constructions.h"
#include "raymath.h"


ColourConfiguration angleBisectorColours = { .line1 = RAYWHITE, .line2 = RED, .arc1 = BLUE};
ColourConfiguration angleBisector_equilateral_colours = {.line1 = RED, .arc1 = ORANGE};

AngleBisectorConstructor InitialiseAngleBisector(Vector2 pointB, Vector2 pointA, Vector2 pointC, float speed, float thickness, ColourConfiguration SelectedColours) {
    AngleBisectorConstructor angleBisector;
    angleBisector.pointB = pointB;
    angleBisector.pointA = pointA;
    angleBisector.pointC = pointC;
    angleBisector.length = (Vector2Distance(pointB, pointA) < Vector2Distance(pointC, pointA)) ? (3*Vector2Distance(pointB, pointA)/4) : (3*Vector2Distance(pointC, pointA)/4); // take 3/4 of the smaller of the two lines
    angleBisector.speed = speed;
    angleBisector.thickness = thickness;
    angleBisector.colours = SelectedColours;
    angleBisector.progress = 0.0f;
    angleBisector.phase = CIRCLEAD;

    angleBisector.angle_BA = atan2(pointB.y - pointA.y , pointB.x - pointA.x);
    angleBisector.pointD = (Vector2){pointA.x + angleBisector.length * cosf(angleBisector.angle_BA), pointA.y + angleBisector.length * sinf(angleBisector.angle_BA)}; // the first intersections of lines with the circle AD
    angleBisector.angle_EA = atan2(pointC.y - pointA.y, pointC.x - pointA.x);
    angleBisector.pointE = (Vector2){pointA.x + angleBisector.length * cosf(angleBisector.angle_EA), pointA.y + angleBisector.length * sinf(angleBisector.angle_EA)}; // second intersection

    angleBisector.circleAD = InitialiseCircle(pointA, angleBisector.pointD, speed, thickness, SelectedColours);
    angleBisector.equilateralED = InitialiseEquilateral(angleBisector.pointE, angleBisector.pointD, angleBisector.pointA, speed, thickness, SelectedColours);

    return angleBisector;
}


void UpdateAngleBisector(AngleBisectorConstructor* angleBisector, float dt) {

    switch (angleBisector->phase) {

        case CIRCLEAD:
            UpdateCircle(&angleBisector->circleAD, dt);
            if (angleBisector->circleAD.complete) angleBisector->phase = AB_LINEDE;
            break;

        case AB_LINEDE:
            angleBisector->progress += angleBisector->speed * dt;
            if (angleBisector->progress >= 1.0f) {
                angleBisector->progress = 0.0f;
                angleBisector->phase = EQUILATERALED;
            }
            break;

        case EQUILATERALED:
            UpdateEquilateral(&angleBisector->equilateralED, dt);
            if (angleBisector->equilateralED.phase == EQUILATERALCOMPLETE) angleBisector->phase = LINEAF;
            break;

        case LINEAF:
            angleBisector->progress += angleBisector->speed * dt;
            if (angleBisector->progress >= 1.0f) {
                angleBisector->progress = 0.0f;
                angleBisector->phase = ANGLEBISECTORCOMPLETE;
            }
            break;

        case ANGLEBISECTORCOMPLETE:
            angleBisector->circleAD.hideMask = CIRCLE_HIDELINE;
            UpdateEquilateral(&angleBisector->equilateralED, dt);

            FadeConstruction(&angleBisector->colours, 0.1f, 0.2f, dt);
            FadeConstruction(&angleBisector->circleAD.colours, 0.1f, 0.2f, dt);
            angleBisector->phase = ANGLEBISECTORCOMPLETE;
            break;
    }
}

void DrawAngleBisectorConstruction(const AngleBisectorConstructor* angleBisector) {

    DrawLineEx(angleBisector->pointA, angleBisector->pointB, angleBisector->thickness, Fade(angleBisector->colours.line1, angleBisector->colours.alpha));
    DrawLineEx(angleBisector->pointA, angleBisector->pointC, angleBisector->thickness, Fade(angleBisector->colours.line1, angleBisector->colours.alpha));

    switch (angleBisector->phase) {

        case CIRCLEAD:
            DrawCircleConstruction(&angleBisector->circleAD);
            break;

        case AB_LINEDE:
            DrawCircleConstruction(&angleBisector->circleAD);

            DrawLineEx(angleBisector->pointD, Vector2Lerp(angleBisector->pointD, angleBisector->pointE, angleBisector->progress), angleBisector->thickness, angleBisector->colours.line1);
            break;

        case EQUILATERALED:
            DrawCircleConstruction(&angleBisector->circleAD);
            DrawEquilateralConstruction(&angleBisector->equilateralED);
            break;

        case LINEAF:
            DrawCircleConstruction(&angleBisector->circleAD);
            DrawEquilateralConstruction(&angleBisector->equilateralED);

            DrawLineEx(angleBisector->pointA, Vector2Lerp(angleBisector->pointA, angleBisector->equilateralED.pointC, angleBisector->progress), angleBisector->thickness, angleBisector->colours.line1);
            break;

        case ANGLEBISECTORCOMPLETE:
            DrawCircleConstruction(&angleBisector->circleAD);
            DrawEquilateralConstruction(&angleBisector->equilateralED);
            DrawLineEx(angleBisector->pointA, angleBisector->equilateralED.pointC, angleBisector->thickness, Fade(angleBisector->colours.line1, angleBisector->colours.alpha));
            break;
    }
}