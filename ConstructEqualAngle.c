#include "constructions.h"
#include "raymath.h"

ColourConfiguration equalAngleColours = {.line1 = WHITE, .arc1 = BLUE};

EqualAngleConstructor InitialiseEqualAngle(Vector2 pointA, Vector2 pointB, Vector2 pointC2, Vector2 pointC1, Vector2 pointC3, float speed, float thickness, ColourConfiguration SelectedColours) {
    EqualAngleConstructor equalAngle;
    equalAngle.pointA = pointA;
    equalAngle.pointB = pointB;
    equalAngle.pointC1 = pointC1;
    equalAngle.pointC2 = pointC2;
    equalAngle.pointC3 = pointC3;
    equalAngle.speed = speed;
    equalAngle.thickness = thickness;
    equalAngle.colours = SelectedColours;
    equalAngle.progress = 0.0f;
    equalAngle.hideMask = 0;
    equalAngle.phase = EA_LINEDE;

    equalAngle.pointD = (Vector2){0.5 * (pointC2.x + pointC1.x), 0.5 * (pointC2.y + pointC1.y)};
    equalAngle.pointE = (Vector2){0.5 * (pointC3.x + pointC1.x), 0.5 * (pointC3.y + pointC1.y)};
    equalAngle.angle_C = atan2(pointC3.y - pointC1.y, pointC3.x - pointC1.x) - atan2(pointC2.y - pointC1.y, pointC2.x - pointC1.x);
    if (equalAngle.angle_C < 0) equalAngle.angle_C += 2 * PI;

    equalAngle.angle_G = atan2f(pointB.y - pointA.y, pointB.x - pointA.x) + equalAngle.angle_C;
    equalAngle.pointG = (Vector2){
        pointA.x + Vector2Distance(pointC1, equalAngle.pointD) * cosf(equalAngle.angle_G),
        pointA.y + Vector2Distance(pointC1, equalAngle.pointD) * sinf(equalAngle.angle_G)
    };

    equalAngle.equalLengthCutCE = InitialiseEqualLengthCut(pointC1, equalAngle.pointE, pointA, pointB, speed, thickness, SelectedColours);
    equalAngle.equalLineCD = InitialiseEqualLine(pointC1, equalAngle.pointD, pointA, speed, thickness, SelectedColours);
    equalAngle.circleAG = InitialiseCircle(pointA, equalAngle.equalLineCD.pointF, speed, thickness, SelectedColours);
    equalAngle.equalLineDE = InitialiseEqualLine(equalAngle.pointD, equalAngle.pointE, equalAngle.equalLengthCutCE.pointF, speed, thickness, SelectedColours);
    equalAngle.circleFG = InitialiseCircle(equalAngle.equalLengthCutCE.pointF, equalAngle.equalLineDE.pointF, speed, thickness, SelectedColours);

    return equalAngle;
}


void UpdateEqualAngle(EqualAngleConstructor *equalAngle, float dt) {

    switch (equalAngle->phase) {
        case EA_LINEDE:
            equalAngle->progress += equalAngle->speed * dt;
            if (equalAngle->progress >= 1.0f) {
                equalAngle->progress = 0.0f;
                equalAngle->phase = EQUALLENGTHCUTCE;
            }
            break;

        case EQUALLENGTHCUTCE:
            UpdateEqualLengthCut(&equalAngle->equalLengthCutCE, dt);
            if (equalAngle->equalLengthCutCE.phase == EQUALLENGTHCUTCOMPLETE) equalAngle->phase = EQUALLINECD;
            break;

        case EQUALLINECD:
            UpdateEqualLine(&equalAngle->equalLineCD, dt);
            if (equalAngle->equalLineCD.phase == EQUALLINECOMPLETE) equalAngle->phase = CIRCLEAG;
            break;

        case CIRCLEAG:
            UpdateEqualLengthCut(&equalAngle->equalLengthCutCE, dt);

            equalAngle->equalLineCD.hideMask = EQUALLINE_HIDEDC;
            UpdateCircle(&equalAngle->circleAG, dt);
            if (equalAngle->circleAG.complete) equalAngle->phase = EQUALLINEDE;
            break;

        case EQUALLINEDE:
            UpdateEqualLengthCut(&equalAngle->equalLengthCutCE, dt);
            UpdateEqualLine(&equalAngle->equalLineCD, dt);

            UpdateEqualLine(&equalAngle->equalLineDE, dt);
            if (equalAngle->equalLineDE.phase == EQUALLINECOMPLETE) equalAngle->phase = CIRCLEFG;
            break;

        case CIRCLEFG:
            UpdateEqualLengthCut(&equalAngle->equalLengthCutCE, dt);
            UpdateEqualLine(&equalAngle->equalLineCD, dt);
            FadeConstruction(&equalAngle->circleAG.colours, 0.1f, 0.2f, dt);

            UpdateCircle(&equalAngle->circleFG, dt);
            if (equalAngle->circleFG.complete) equalAngle->phase = TRIANGLEAGF;
            break;

        case TRIANGLEAGF:
            UpdateEqualLengthCut(&equalAngle->equalLengthCutCE, dt);
            UpdateEqualLine(&equalAngle->equalLineCD, dt);
            FadeConstruction(&equalAngle->circleAG.colours, 0.1f, 0.2f, dt);
            UpdateEqualLine(&equalAngle->equalLineDE, dt);

            equalAngle->progress += equalAngle->speed * dt;
            if (equalAngle->progress >= 1.0f) {
                equalAngle->progress = 0.0f;
                equalAngle->phase = EQUALANGLECOMPLETE;
            }
            break;

        case EQUALANGLECOMPLETE:
            UpdateEqualLengthCut(&equalAngle->equalLengthCutCE, dt);
            UpdateEqualLine(&equalAngle->equalLineCD, dt);
            FadeConstruction(&equalAngle->circleAG.colours, 0.2f, 0.1f, dt);
            UpdateEqualLine(&equalAngle->equalLineDE, dt);
            FadeConstruction(&equalAngle->circleFG.colours, 0.2f, 0.1f, dt);
            FadeConstruction(&equalAngle->colours, 0.2f, 0.1f, dt);
            equalAngle->phase = EQUALANGLECOMPLETE;
            break;
    }
}


void DrawEqualAngleConstruction(const EqualAngleConstructor *equalAngle) {

    switch (equalAngle->phase) {
        case EA_LINEDE:
            DrawLineEx(equalAngle->pointE, Vector2Lerp(equalAngle->pointE, equalAngle->pointC1, equalAngle->progress), equalAngle->thickness, equalAngle->colours.line1);
            DrawLineEx(equalAngle->pointC1, Vector2Lerp(equalAngle->pointC1, equalAngle->pointD, equalAngle->progress), equalAngle->thickness, equalAngle->colours.line1);
            DrawLineEx(equalAngle->pointD, Vector2Lerp(equalAngle->pointD, equalAngle->pointE, equalAngle->progress), equalAngle->thickness, equalAngle->colours.line1);
            break;

        case EQUALLENGTHCUTCE:
            DrawLineEx(equalAngle->pointC1, equalAngle->pointD, equalAngle->thickness, equalAngle->colours.line1);
            DrawLineEx(equalAngle->pointD, equalAngle->pointE, equalAngle->thickness, equalAngle->colours.line1);
            DrawEqualLengthCutConstruction(&equalAngle->equalLengthCutCE);
            break;

        case EQUALLINECD:
            DrawLineEx(equalAngle->pointD, equalAngle->pointE, equalAngle->thickness, equalAngle->colours.line1);
            DrawEqualLengthCutConstruction(&equalAngle->equalLengthCutCE);
            DrawEqualLineConstruction(&equalAngle->equalLineCD);
            break;

        case CIRCLEAG:
            DrawLineEx(equalAngle->pointC1, equalAngle->pointD, equalAngle->thickness, equalAngle->colours.line1);
            DrawLineEx(equalAngle->pointD, equalAngle->pointE, equalAngle->thickness, equalAngle->colours.line1);
            DrawEqualLengthCutConstruction(&equalAngle->equalLengthCutCE);
            DrawEqualLineConstruction(&equalAngle->equalLineCD);
            DrawCircleConstruction(&equalAngle->circleAG);
            break;

        case EQUALLINEDE:
            DrawLineEx(equalAngle->pointC1, equalAngle->pointD, equalAngle->thickness, equalAngle->colours.line1);
            DrawLineEx(equalAngle->pointD, equalAngle->pointE, equalAngle->thickness, equalAngle->colours.line1);
            DrawEqualLengthCutConstruction(&equalAngle->equalLengthCutCE);
            DrawEqualLineConstruction(&equalAngle->equalLineCD);
            DrawCircleConstruction(&equalAngle->circleAG);
            DrawEqualLineConstruction(&equalAngle->equalLineDE);
            break;

        case CIRCLEFG:
            DrawLineEx(equalAngle->pointC1, equalAngle->pointD, equalAngle->thickness, equalAngle->colours.line1);
            DrawLineEx(equalAngle->pointD, equalAngle->pointE, equalAngle->thickness, equalAngle->colours.line1);
            DrawEqualLengthCutConstruction(&equalAngle->equalLengthCutCE);
            DrawEqualLineConstruction(&equalAngle->equalLineCD);
            DrawCircleConstruction(&equalAngle->circleAG);
            DrawEqualLineConstruction(&equalAngle->equalLineDE);
            DrawCircleConstruction(&equalAngle->circleFG);
            break;

        case TRIANGLEAGF:
            DrawLineEx(equalAngle->pointC1, equalAngle->pointD, equalAngle->thickness, equalAngle->colours.line1);
            DrawLineEx(equalAngle->pointD, equalAngle->pointE, equalAngle->thickness, equalAngle->colours.line1);
            DrawEqualLengthCutConstruction(&equalAngle->equalLengthCutCE);
            DrawEqualLineConstruction(&equalAngle->equalLineCD);
            DrawCircleConstruction(&equalAngle->circleAG);
            DrawEqualLineConstruction(&equalAngle->equalLineDE);
            DrawCircleConstruction(&equalAngle->circleFG);

            DrawLineEx(equalAngle->pointA, Vector2Lerp(equalAngle->pointA, equalAngle->pointG, equalAngle->progress),
                       equalAngle->thickness, equalAngle->colours.line1);
            DrawLineEx(equalAngle->equalLengthCutCE.pointF,
                       Vector2Lerp(equalAngle->equalLengthCutCE.pointF, equalAngle->pointG, equalAngle->progress),
                       equalAngle->thickness, equalAngle->colours.line1);
            break;

        case EQUALANGLECOMPLETE:
            if (!(equalAngle->hideMask & EQUALANGLE_HIDEC1D))
                DrawLineEx(equalAngle->pointC1, equalAngle->pointD, equalAngle->thickness, equalAngle->colours.line1);
            if (!(equalAngle->hideMask & EQUALANGLE_HIDEDE))
                DrawLineEx(equalAngle->pointD, equalAngle->pointE, equalAngle->thickness, equalAngle->colours.line1);
            DrawEqualLengthCutConstruction(&equalAngle->equalLengthCutCE);
            DrawEqualLineConstruction(&equalAngle->equalLineCD);
            DrawCircleConstruction(&equalAngle->circleAG);
            DrawEqualLineConstruction(&equalAngle->equalLineDE);
            DrawCircleConstruction(&equalAngle->circleFG);
            if (!(equalAngle->hideMask & EQUALANGLE_HIDEAG))
                DrawLineEx(equalAngle->pointA, equalAngle->pointG, equalAngle->thickness, equalAngle->colours.line1);
            if (!(equalAngle->hideMask & EQUALANGLE_HIDEFG))
                DrawLineEx(equalAngle->equalLengthCutCE.pointF, equalAngle->pointG, equalAngle->thickness,equalAngle->colours.line1);
    }
}
