#include "constructions.h"
#include <math.h>

void ConstructCircle(Vector2 centre,
                                  float radius,
                                  float *angle,
                                  float speed,
                                  float thickness,
                                  Color circleColor,
                                  Color radiusColor)
{
    // updating angle while drawing
    if (*angle < 2*PI)
    {
        *angle += speed * GetFrameTime();
        if (*angle > 2*PI) *angle = 2*PI;
    }

    // manually drawing circle with lines while rotation is ongoing
    const int segments = 128;
    float maxAngle = *angle;
    float step = maxAngle / segments;

    for (int i = 0; i < segments; i++)
    {
        float a1 = i * step;
        float a2 = (i + 1) * step;

        Vector2 p1 = {
            centre.x + cosf(a1) * radius,
            centre.y + sinf(a1) * radius
        };
        Vector2 p2 = {
            centre.x + cosf(a2) * radius,
            centre.y + sinf(a2) * radius
        };

        DrawLineEx(p1, p2, thickness, circleColor);
    }

    // drawing radius arm
    float armAngle = (*angle < 2*PI) ? *angle : 0.0f;

    Vector2 armEnd = {
        centre.x + cosf(armAngle) * radius,
        centre.y + sinf(armAngle) * radius
    };

    DrawLineEx(centre, armEnd, thickness, radiusColor);
}
