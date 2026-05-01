#include <external/stb_truetype.h>

#include "constructions.h"
#include "raymath.h"

ColourConfiguration equalLineColours = { .line1 = WHITE, .arc1 = BLUE};


EqualLineConstructor InitialiseEqualLine(Vector2 pointA, Vector2 pointB, Vector2 pointC, float speed, float thickness, ColourConfiguration SelectedColours) {
    EqualLineConstructor equalLine;
    equalLine.pointA = pointA;
    equalLine.pointB = pointB;
    equalLine.pointC = pointC;
    equalLine.length = Vector2Distance(pointA, pointB);
    equalLine.speed = speed;
    equalLine.thickness = thickness;
    equalLine.colours = SelectedColours;
    equalLine.progress = 0.0f;
    equalLine.phase = LINEBC;
    equalLine.hideMask = 0;

    equalLine.equilateralBC = InitialiseEquilateral(pointB, pointC, (Vector2){0}, speed, thickness, SelectedColours);
    equalLine.circleBA = InitialiseCircle(pointB, pointA, speed, thickness, SelectedColours);

    // needs to be in this order since it references points from within the equilateral.

    equalLine.angle_BE = atan2(equalLine.equilateralBC.pointC.y - pointB.y, equalLine.equilateralBC.pointC.x - pointB.x) + PI; // angle for DB extension
    equalLine.angle_CF = atan2(equalLine.equilateralBC.pointC.y - pointC.y, equalLine.equilateralBC.pointC.x - pointC.x) + PI; // angle for DC extension
    equalLine.pointE = (Vector2){pointB.x + equalLine.length * cosf(equalLine.angle_BE), pointB.y + equalLine.length * sinf(equalLine.angle_BE)}; // point to which DB extends
    equalLine.pointF = (Vector2){pointC.x + equalLine.length * cosf(equalLine.angle_CF), pointC.y + equalLine.length * sinf(equalLine.angle_CF)}; // point to which DC extends

    equalLine.circleDE = InitialiseCircle(equalLine.equilateralBC.pointC, equalLine.pointE, speed, thickness, SelectedColours);

    return equalLine;
}


void UpdateEqualLine(EqualLineConstructor* equalLine, float dt) {

    switch (equalLine->phase) {

        case LINEBC:
            equalLine->progress += equalLine->speed * dt;
            if (equalLine->progress >= 1.0f) {
                equalLine->progress = 0.0f;
                equalLine->phase = EQUILATERALBC;
            }
            break;

        case EQUILATERALBC:
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
            UpdateEquilateral(&equalLine->equilateralBC, dt);

            FadeConstruction(&equalLine->colours, 0.1f, 0.2f, dt);
            FadeConstruction(&equalLine->circleBA.colours, 0.1f, 0.2f, dt);
            FadeConstruction(&equalLine->circleDE.colours, 0.1f, 0.2f, dt);
            equalLine->phase = EQUALLINECOMPLETE;
            break;
        }
}


void DrawEqualLineConstruction(const EqualLineConstructor* equalLine) {

    switch (equalLine->phase) {

        case LINEBC:
            DrawLineEx(equalLine->pointA, equalLine->pointB, equalLine->thickness, equalLine->colours.line1);
            DrawLineEx(equalLine->pointB, Vector2Lerp(equalLine->pointB, equalLine->pointC, equalLine->progress), equalLine->thickness, equilateralColours.line1);
            break;

        case EQUILATERALBC:
            DrawLineEx(equalLine->pointA, equalLine->pointB, equalLine->thickness, equalLine->colours.line1);

            DrawEquilateralConstruction(&equalLine->equilateralBC);
            break;

        case CIRCLEBA:
            DrawEquilateralConstruction(&equalLine->equilateralBC);
            DrawCircleConstruction(&equalLine->circleBA);
            break;

        case EXTENDDB:
            DrawEquilateralConstruction(&equalLine->equilateralBC);
            DrawCircleConstruction(&equalLine->circleBA);

            DrawLineEx(equalLine->pointB, Vector2Lerp(equalLine->pointB, equalLine->pointE, equalLine->progress), equalLine->thickness, equilateralColours.line1);
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

            DrawLineEx(equalLine->pointC, Vector2Lerp(equalLine->pointC, equalLine->pointF, equalLine->progress), equalLine->thickness, equalLine->colours.line1);
            break;

        case EQUALLINECOMPLETE:
            if (!(equalLine->hideMask & EQUALLINE_HIDEDC))
                DrawLineEx(equalLine->pointC, equalLine->pointF, equalLine->thickness, Fade(equalLine->colours.line1, equalLine->colours.alpha));

            DrawEquilateralConstruction(&equalLine->equilateralBC);
            DrawCircleConstruction(&equalLine->circleBA);
            DrawCircleConstruction(&equalLine->circleDE);
            break;
    }
}

