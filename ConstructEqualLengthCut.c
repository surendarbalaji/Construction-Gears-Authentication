#include "constructions.h"
#include "raymath.h"

ColourConfiguration equalLengthCutColours = {.line1 = WHITE, .arc1 = BLUE};

EqualLengthCutConstructor InitialiseEqualLengthCut(Vector2 pointA, Vector2 pointB, Vector2 pointC, Vector2 pointD, float speed, float thickness, ColourConfiguration SelectedColours) {
    EqualLengthCutConstructor equalLengthCut;
    equalLengthCut.pointA = pointA;
    equalLengthCut.pointB = pointB;
    equalLengthCut.pointC = pointC;
    equalLengthCut.pointD = pointD;
    equalLengthCut.speed = speed;
    equalLengthCut.thickness = thickness;
    equalLengthCut.colours = SelectedColours;
    equalLengthCut.progress = 0.0f;
    equalLengthCut.phase = EQUALLINE;

    equalLengthCut.equalLineAB = InitialiseEqualLine(pointA, pointB, pointC, speed, thickness, SelectedColours);
    equalLengthCut.circleCF = InitialiseCircle(pointC, equalLengthCut.equalLineAB.pointF, speed, thickness, SelectedColours);

    equalLengthCut.angle_CD = atan2(pointD.y - pointC.y, pointD.x - pointC.x);
    equalLengthCut.pointF = (Vector2){pointC.x + equalLengthCut.equalLineAB.length * cosf(equalLengthCut.angle_CD), pointC.y + equalLengthCut.equalLineAB.length * sinf(equalLengthCut.angle_CD)};

    return equalLengthCut;
}


void UpdateEqualLengthCut(EqualLengthCutConstructor* equalLengthCut, float dt) {

    switch (equalLengthCut->phase) {

        case EQUALLINE:
            UpdateEqualLine(&equalLengthCut->equalLineAB, dt);
            if (equalLengthCut->equalLineAB.phase == EQUALLINECOMPLETE) equalLengthCut->phase = CIRCLECE;
            break;

        case CIRCLECE:
            equalLengthCut->equalLineAB.hideMask = EQUALLINE_HIDEDC;
            UpdateCircle(&equalLengthCut->circleCF, dt);
            if (equalLengthCut->circleCF.complete) equalLengthCut->phase = LINECF;
            break;

        case LINECF:
            equalLengthCut->progress += equalLengthCut->speed * dt;
            if (equalLengthCut->progress >= 1.0f) {
                equalLengthCut->progress = 0.0f;
                equalLengthCut->phase = EQUALLENGTHCUTCOMPLETE;
            }
            break;

        case EQUALLENGTHCUTCOMPLETE:
            UpdateEqualLine(&equalLengthCut->equalLineAB, dt);

            FadeConstruction(&equalLengthCut->equalLineAB.colours, 0.1f, 0.2f, dt);
            FadeConstruction(&equalLengthCut->colours, 0.1f, 0.2f, dt);
            FadeConstruction(&equalLengthCut->circleCF.colours, 0.1f, 0.2f, dt);
            equalLengthCut->phase = EQUALLENGTHCUTCOMPLETE;
            break;
        }
}


void DrawEqualLengthCutConstruction(const EqualLengthCutConstructor* equalLengthCut) {

    switch (equalLengthCut->phase) {

        case EQUALLINE:
            DrawEqualLineConstruction(&equalLengthCut->equalLineAB);
            break;

        case CIRCLECE:
            DrawEqualLineConstruction(&equalLengthCut->equalLineAB);
            DrawCircleConstruction(&equalLengthCut->circleCF);
            break;

        case LINECF:
            DrawEqualLineConstruction(&equalLengthCut->equalLineAB);
            DrawCircleConstruction(&equalLengthCut->circleCF);
            DrawLineEx(equalLengthCut->pointC, Vector2Lerp(equalLengthCut->pointC, equalLengthCut->pointF, equalLengthCut->progress), equalLengthCut->thickness, equalLengthCut->colours.line1);
            break;

        case EQUALLENGTHCUTCOMPLETE:
            DrawEqualLineConstruction(&equalLengthCut->equalLineAB);
            DrawCircleConstruction(&equalLengthCut->circleCF);
            DrawLineEx(equalLengthCut->pointC, equalLengthCut->pointF, equalLengthCut->thickness, Fade(equalLengthCut->colours.line1, equalLengthCut->colours.alpha));
            break;
    }
}