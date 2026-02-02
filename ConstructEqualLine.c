#include "constructions.h"
#include "raymath.h"

EqualLineConstructor InitialiseEqualLine(Vector2 pointA, Vector2 pointB, Vector2 pointC, float speed, float thickness, Color lengthColour, Color arcColour, Color equilateralLineColour, Color equilateralArcColour) {
    EqualLineConstructor equalLine;
    equalLine.pointA = pointA;
    equalLine.pointB = pointB;
    equalLine.pointC = pointC;
    equalLine.length = Vector2Distance(pointA, pointB);
    equalLine.speed = speed;
    equalLine.thickness = thickness;
    equalLine.lengthColour = lengthColour;
    equalLine.arcColour = arcColour;
    equalLine.equilateralLineColour = equilateralLineColour;
    equalLine.equilateralArcColour = equilateralArcColour;
    equalLine.progress = 0.0f;
    equalLine.phase = LINEBC;

    equalLine.equilateralBC = InitialiseEquilateral(pointB, pointC, (Vector2){0}, speed, thickness, equilateralArcColour, equilateralLineColour);
    equalLine.circleBA = InitialiseCircle(pointB, pointA, speed, thickness, arcColour, lengthColour);

    // needs to be in this order since it references points from within the equilateral.

    equalLine.angle_BE = atan2(equalLine.equilateralBC.pointC.y - pointB.y, equalLine.equilateralBC.pointC.x - pointB.x) + PI; // angle for DB extension
    equalLine.angle_CF = atan2(equalLine.equilateralBC.pointC.y - pointC.y, equalLine.equilateralBC.pointC.x - pointC.x) + PI; // angle for DC extension
    equalLine.pointE = (Vector2){pointB.x + equalLine.length * cosf(equalLine.angle_BE), pointB.y + equalLine.length * sinf(equalLine.angle_BE)}; // point to which DB extends
    equalLine.pointF = (Vector2){pointC.x + equalLine.length * cosf(equalLine.angle_CF), pointC.y + equalLine.length * sinf(equalLine.angle_CF)}; // point to which DC extends

    equalLine.circleDE = InitialiseCircle(equalLine.equilateralBC.pointC, equalLine.pointE, speed, thickness, arcColour, equilateralLineColour);

    return equalLine;
}


void UpdateEqualLine(EqualLineConstructor* equalLine, float dt) {
    if (equalLine->phase == EQUALLINECOMPLETE) return;

    switch (equalLine->phase) {

        case LINEBC:
            equalLine->progress += equalLine->speed * dt;
            if (equalLine->progress >= 1.0f) {
                equalLine->progress = 0.0f;
                equalLine->phase = EQUILATERAL;
            }
            break;

        case EQUILATERAL:
            UpdateEquilateral(&equalLine->equilateralBC, dt);
            if (equalLine->equilateralBC.phase == EQUILATERALCOMPLETE) equalLine->phase = CIRCLEBA;
            break;

        case CIRCLEBA:
            UpdateCircle(&equalLine->circleBA, dt);
            if (equalLine->circleBA.complete) equalLine->phase = EXTENDDB;
            break;

        case EXTENDDB:
            equalLine->progress += equalLine->speed * dt;
            if (equalLine->progress >= 1.0f) {
                equalLine->progress = 0.0f;
                equalLine->phase = CIRCLEDE;
            }
            break;

        case CIRCLEDE:
            equalLine->equilateralBC.hideMask = EQUILATERAL_HIDEAC;
            UpdateCircle(&equalLine->circleDE, dt);
            if (equalLine->circleDE.complete) equalLine->phase = EXTENDDC;
            break;

        case EXTENDDC:
            equalLine->progress += equalLine->speed * dt;
            if (equalLine->progress >= 1.0f) {
                equalLine->progress = 0.0f;
                equalLine->phase = EQUALLINECOMPLETE;
            }
            break;

        case EQUALLINECOMPLETE:
            equalLine->phase = EQUALLINECOMPLETE;
            break;
        }
}


void DrawEqualLineConstruction(const EqualLineConstructor* equalLine) {

    switch (equalLine->phase) {

        case LINEBC:
            DrawLineEx(equalLine->pointA, equalLine->pointB, equalLine->thickness, equalLine->lengthColour);
            DrawLineEx(equalLine->pointB, Vector2Lerp(equalLine->pointB, equalLine->pointC, equalLine->progress), equalLine->thickness, equalLine->equilateralLineColour);
            break;

        case EQUILATERAL:
            DrawLineEx(equalLine->pointA, equalLine->pointB, equalLine->thickness, equalLine->lengthColour);

            DrawEquilateralConstruction(&equalLine->equilateralBC);
            break;

        case CIRCLEBA:
            DrawEquilateralConstruction(&equalLine->equilateralBC);
            DrawCircleConstruction(&equalLine->circleBA);
            break;

        case EXTENDDB:
            DrawEquilateralConstruction(&equalLine->equilateralBC);
            DrawCircleConstruction(&equalLine->circleBA);

            DrawLineEx(equalLine->pointB, Vector2Lerp(equalLine->pointB, equalLine->pointE, equalLine->progress), equalLine->thickness, equalLine->equilateralLineColour);
            break;

        case CIRCLEDE:
            DrawEquilateralConstruction(&equalLine->equilateralBC);
            DrawCircleConstruction(&equalLine->circleBA);
            DrawCircleConstruction(&equalLine->circleDE);

            // DrawLineEx(equalLine->equilateralBC.pointC, equalLine->pointE, equalLine->thickness, BLACK);
            break;

        case EXTENDDC:
            DrawEquilateralConstruction(&equalLine->equilateralBC);
            DrawCircleConstruction(&equalLine->circleBA);
            DrawCircleConstruction(&equalLine->circleDE);

            DrawLineEx(equalLine->pointC, Vector2Lerp(equalLine->pointC, equalLine->pointF, equalLine->progress), equalLine->thickness, equalLine->lengthColour);
            break;

        case EQUALLINECOMPLETE:
            DrawLineEx(equalLine->pointC, equalLine->pointF, equalLine->thickness, equalLine->lengthColour);

            DrawEquilateralConstruction(&equalLine->equilateralBC);
            DrawCircleConstruction(&equalLine->circleBA);
            DrawCircleConstruction(&equalLine->circleDE);
            break;
    }
}

