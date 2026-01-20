#include "constructions.h"
#include "raymath.h"
#include <math.h>

// initialise struct
BisectorConstructor InitialiseBisector(Vector2 point1, Vector2 point2, float speed, float thickness, Color arcColour, Color lineColour) {
    BisectorConstructor bisector;
    bisector.point1 = point1;
    bisector.point2 = point2;
    bisector.length = Vector2Distance(point1, point2);
    bisector.speed = speed;
    bisector.thickness = thickness;
    bisector.arcColour = arcColour;
    bisector.lineColour = lineColour;
    bisector.initial_angle = atan2f(point2.y - point1.y, point2.x - point1.x);
    bisector.bisector_angle = bisector.initial_angle + PI/2;
    bisector.progress = 0.0f;
    bisector.complete = false;
    return bisector;
}

void UpdateBisector(BisectorConstructor* bisector, float dt) {
    if (!bisector->complete) {
        bisector->progress += bisector->speed *dt;

        // i want the arc to span just over 2pi/3 to ensure intersection and a slight hangover, hence the angle limit is changed to 4pi/5
        if (bisector->progress >= 0.8f * PI) {
            bisector->progress = 0.8f * PI;
            bisector->complete = true;
        }
    }
}

void DrawBisectorConstruction(const BisectorConstructor* bisector) {
    const int segments = 100;
    float step = bisector->progress / segments;

    for (int i = 0; i < segments; i++) {

        // arc of circle point1 to point2
        // to start the drawing from the edge of the arc instead of the center, i've just subtracted half of the total angle (there's probably a much better way to do this (and everything here for that matter))
        float angle1 = bisector->initial_angle - (0.4f * PI)  + i * step;
        float angle2 = bisector->initial_angle - (0.4f * PI) + (i + 1) * step;

        Vector2 p1 = {
            bisector->point1.x + bisector->length * cosf(angle1),
            bisector->point1.y + bisector->length * sinf(angle1)
        };
        Vector2 p2 = {
            bisector->point1.x + bisector->length * cosf(angle2),
            bisector->point1.y + bisector->length * sinf(angle2)
        };

        DrawLineEx(p1, p2, bisector->thickness, bisector->arcColour);

        // arc of circle point2 to point1
        float angle3 = bisector->initial_angle - (0.4f * PI)  + i * step;
        float angle4 = bisector->initial_angle - (0.4f * PI) + (i + 1) * step;

        Vector2 p3 = {
            bisector->point2.x + bisector->length * -cosf(angle3),
            bisector->point2.y + bisector->length * -sinf(angle3)
        };
        Vector2 p4 = {
            bisector->point2.x + bisector->length * -cosf(angle4),
            bisector->point2.y + bisector->length * -sinf(angle4)
        };

        DrawLineEx(p3, p4, bisector->thickness, bisector->arcColour);
    }

    float arm_angle = bisector->initial_angle - (0.4f * PI) + bisector->progress;
    Vector2 arm_end = {
        bisector->point1.x + bisector->length * cosf(arm_angle),
        bisector->point1.y + bisector->length * sinf(arm_angle)
    };

    DrawLineEx(bisector->point1, arm_end, bisector->thickness, bisector->lineColour);

    // TRIGONOMETRY !!!!!
    // we are finding the length of the new bisector line by multiplying length between our two points by sin pi/3 (this angle is constant since the arcs have radius equal to length between points)
    // this is multiplied the sine and cosine of the bisector angle to find the change in x and y respectively, then finally added to the midpoint
    Vector2 bisectorPoint = {
        (bisector->point1.x + bisector->point2.x) / 2 + (bisector->progress * bisector->length * sinf(PI/3) * cosf(bisector->bisector_angle) / (0.8 * PI)),
        (bisector->point1.y + bisector->point2.y) / 2 + (bisector->progress * bisector->length * sinf(PI/3) * sinf(bisector->bisector_angle) / (0.8 * PI))
    };

    // same thing, opposite direction
    Vector2 bisectorPoint2 = {
        (bisector->point1.x + bisector->point2.x) / 2 - (bisector->progress * bisector->length * sinf(PI/3) * cosf(bisector->bisector_angle) / (0.8 * PI)),
        (bisector->point1.y + bisector->point2.y) / 2 - (bisector->progress * bisector->length * sinf(PI/3) * sinf(bisector->bisector_angle) / (0.8 * PI))
    };

    DrawLineEx(bisectorPoint, bisectorPoint2, bisector->thickness, GREEN);
}