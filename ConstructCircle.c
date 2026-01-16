#include "constructions.h"
#include "raymath.h"
#include <math.h>


// initialise all values that don't change for the same circle
CircleConstructor InitialiseCircle(Vector2 centre, Vector2 point, float speed, float thickness, Color circleColor, Color radiusColor) {
    CircleConstructor circle;
    circle.centre = centre;
    circle.point = point;
    circle.radius = Vector2Distance(centre, point);
    circle.speed = speed;
    circle.thickness = thickness;
    circle.circleColor = circleColor;
    circle.radiusColor = radiusColor;
    circle.initial_angle = atan2f(point.y - centre.y, point.x - centre.x);
    circle.progress = 0.0f;
    circle.complete = false;
    return circle;
}

// seperate function to update the progress of the arc that can be called before the drawing loop
void UpdateCircle(CircleConstructor* circle, float dt) {
    if (!circle->complete) {
        circle->progress += circle->speed * dt;
        if (circle->progress >= 2.0f * PI) {
            circle->progress = 2.0f * PI;
            circle->complete = true;
        }
    }
}

// same manual method for drawing circle with ongoing rotation
void DrawCircleConstruction(const CircleConstructor* circle) {
    const int segments = 100;
    float step = circle->progress / segments;

    for (int i = 0; i < segments; i++) {
        float angle1 = circle->initial_angle + i * step;
        float angle2 = circle->initial_angle + (i + 1) * step;

        Vector2 p1 = {
            circle->centre.x + circle->radius * cosf(angle1),
            circle->centre.y + circle->radius * sinf(angle1)
        };
        Vector2 p2 = {
            circle->centre.x + circle->radius * cosf(angle2),
            circle->centre.y + circle->radius * sinf(angle2)
        };

        DrawLineEx(p1, p2, circle->thickness, circle->circleColor);
    }

    float arm_angle = circle->initial_angle + circle->progress;
    Vector2 arm_end = {
        circle->centre.x + circle->radius * cosf(arm_angle),
        circle->centre.y + circle->radius * sinf(arm_angle)
    };

    DrawLineEx(circle->centre, arm_end, circle->thickness, circle->radiusColor);
}