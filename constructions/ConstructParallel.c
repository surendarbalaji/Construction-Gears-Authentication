#include "constructions.h"
#include "raymath.h"

ColourConfiguration parallelColours = {.line1 = WHITE, .arc1 = BLUE};

ParallelConstructor InitialiseParallel(Vector2 pointA, Vector2 pointB, Vector2 pointC, float speed, float thickness, ColourConfiguration parallelColours) {
    ParallelConstructor parallel;
    parallel.pointA = pointA;
    parallel.pointB = pointB;
    parallel.pointC = pointC;
    parallel.pointD = (Vector2){0.5 * (pointB.x + pointC.x), 0.5 * (pointB.y + pointC.y)};
    parallel.length = Vector2Distance(parallel.pointD, pointC);
    parallel.speed = speed;
    parallel.thickness = thickness;
    parallel.colours = parallelColours;
    parallel.progress = 0.0f;
    parallel.phase = LINEAD;

    parallel.equalAngleADC = InitialiseEqualAngle(pointA, parallel.pointD, pointA, parallel.pointD, pointC, speed, thickness, parallelColours);

    parallel.angle_G = atan2(parallel.equalAngleADC.pointG.y - pointA.y, parallel.equalAngleADC.pointG.x - pointA.x);
    parallel.pointF1 = (Vector2){pointA.x + parallel.length * cosf(parallel.angle_G), pointA.y + parallel.length * sinf(parallel.angle_G)};
    parallel.pointF2 = (Vector2){pointA.x + parallel.length * cosf(parallel.angle_G + PI), pointA.y + parallel.length * sinf(parallel.angle_G + PI)};

    return parallel;
}


void UpdateParallel(ParallelConstructor* parallel, float dt) {

    switch (parallel->phase) {

        case LINEAD:
            parallel->progress += parallel->speed * dt;
            if (parallel->progress >= 1.0f) {
                parallel->progress = 0.0f;
                parallel->phase = EQUALANGLE;
            }
            break;

        case EQUALANGLE:
            UpdateEqualAngle(&parallel->equalAngleADC, dt);
            if (parallel->equalAngleADC.phase == EQUALANGLECOMPLETE) parallel->phase = P_LINEAF;
            break;

        case P_LINEAF:
            UpdateEqualAngle(&parallel->equalAngleADC, dt);
            parallel->progress += parallel->speed * dt;
            if (parallel->progress >= 1.0f) {
                parallel->progress = 0.0f;
                parallel->phase = PARALLELCOMPLETE;
            }
            break;

        case PARALLELCOMPLETE:
            parallel->equalAngleADC.hideMask = EQUALANGLE_HIDEC1D;
            parallel->equalAngleADC.hideMask = EQUALANGLE_HIDEAG;
            parallel->equalAngleADC.hideMask = EQUALANGLE_HIDEFG;
            parallel->equalAngleADC.hideMask = EQUALANGLE_HIDEDE;

            UpdateEqualAngle(&parallel->equalAngleADC, dt);
            FadeConstruction(&parallel->colours, 0.1f, 0.2f, dt);
            parallel->phase = PARALLELCOMPLETE;
            break;
    }
}


void DrawParallelConstruction(const ParallelConstructor* parallel) {

    DrawLineEx(parallel->pointB, parallel->pointC, parallel->thickness, RAYWHITE);
    // DrawCircleV(parallel->pointA, 10.0f, RAYWHITE);
    // DrawCircleV(parallel->equalAngleADC.pointG, 10.0f, RAYWHITE);

    switch (parallel->phase) {

        case LINEAD:
            DrawLineEx(parallel->pointA, Vector2Lerp(parallel->pointA, parallel->pointD, parallel->progress), parallel->thickness, parallel->colours.line1);
            break;

        case EQUALANGLE:
            DrawLineEx(parallel->pointA, parallel->pointD, parallel->thickness, parallel->colours.line1);
            DrawEqualAngleConstruction(&parallel->equalAngleADC);
            break;

        case P_LINEAF:
            DrawLineEx(parallel->pointA, parallel->pointD, parallel->thickness, parallel->colours.line1);
            DrawEqualAngleConstruction(&parallel->equalAngleADC);

            DrawLineEx(parallel->pointA, Vector2Lerp(parallel->pointA, parallel->pointF1, parallel->progress), parallel->thickness, WHITE);
            DrawLineEx(parallel->pointA, Vector2Lerp(parallel->pointA, parallel->pointF2, parallel->progress), parallel->thickness, WHITE);

            break;

        case PARALLELCOMPLETE:
            // DrawLineEx(parallel->pointA, parallel->pointD, parallel->thickness, Fade(parallel->colours.line1, parallel->colours.alpha));
            DrawEqualAngleConstruction(&parallel->equalAngleADC);
            DrawLineEx(parallel->pointF1, parallel->pointF2, parallel->thickness, WHITE);
            break;
    }
}
