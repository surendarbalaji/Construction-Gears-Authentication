#include "constructions.h"
#include "raymath.h"


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
    bisector.phase = ARC1_CLOCKWISE;
    return bisector;
}

void UpdateBisector(BisectorConstructor* bisector, float dt) {

    bisector->progress += bisector->speed *dt;

    // arc construction is split into nine sequences of arc drawing, the update function now cycles through them
    if (bisector->progress >= 1.0f) {
        bisector->progress = 0.0f;

        switch (bisector->phase) {
            case ARC1_CLOCKWISE:
                bisector->phase = ARC1_WIDDERSHINS;
                break;
            case ARC1_WIDDERSHINS:
                bisector->phase = ARC2_WIDDERSHINS;
                break;
            case ARC2_WIDDERSHINS:
                bisector->phase = ARC2_CLOCKWISE;
                break;
            case ARC2_CLOCKWISE:
                bisector->phase = ARC3_CLOCKWISE;
                break;
            case ARC3_CLOCKWISE:
                bisector->phase = ARC3_WIDDERSHINS;
                break;
            case ARC3_WIDDERSHINS:
                bisector->phase = ARC4_CLOCKWISE;
                break;
            case ARC4_CLOCKWISE:
                bisector->phase = ARC4_WIDDERSHINS;
                break;
            case ARC4_WIDDERSHINS:
                bisector->phase = BISECTION;
                break;
            case BISECTION:
                bisector->phase = BISECTORCOMPLETE;
                case BISECTORCOMPLETE:
                bisector->phase = BISECTORCOMPLETE;
                break;
            }
        }
    }

// this function can draw one arc given the starting angle, so it can be reused in all the previous cases

void DrawArc(Vector2 centre, float radius, float initial_angle, float rotation_direction, float progress, float thickness, Color arcColour, Color lineColour) {
    float end_angle = (360 * initial_angle) / (2 * PI) + rotation_direction * 72 * progress;

    // wait, raylib has a draw radius function??? and it uses degrees??? what??
    DrawRing(centre, radius, radius + thickness, (360 * initial_angle) / (2 * PI), end_angle, 100, arcColour);

    float arm_angle = initial_angle + rotation_direction * 0.4f * PI * progress;
    Vector2 arm_end = {
        centre.x + radius * cosf(arm_angle),
        centre.y + radius * sinf(arm_angle)
    };

    DrawLineEx(centre, arm_end, thickness, lineColour);
}

void DrawBisectingLine(Vector2 point1, Vector2 point2, float length, float bisector_angle, float progress, float thickness, Color lineColour) {

    // TRIGONOMETRY !!!!! (drawing perpendicular bisector itself)
    // we are finding the length of the new bisector line by multiplying length between our two points by sin pi/3 (this angle is constant since the arcs have radius equal to length between points)
    // this is multiplied the sine and cosine of the bisector angle to find the change in x and y respectively, then finally added to the midpoint
    Vector2 bisectorPoint = {
        (point1.x + point2.x) / 2 + (progress * length * sinf(PI/3) * cosf(bisector_angle) * 1.2),
        (point1.y + point2.y) / 2 + (progress * length * sinf(PI/3) * sinf(bisector_angle) * 1.2)
    };

    // same thing, opposite direction
    Vector2 bisectorPoint2 = {
        (point1.x + point2.x) / 2 - (progress * length * sinf(PI/3) * cosf(bisector_angle) * 1.2),
        (point1.y + point2.y) / 2 - (progress * length * sinf(PI/3) * sinf(bisector_angle) * 1.2)
    };

    DrawLineEx(bisectorPoint, bisectorPoint2, thickness, lineColour);

}

void DrawBisectorConstruction(const BisectorConstructor* bisector) {

    switch (bisector->phase) {
        case ARC1_CLOCKWISE:

            DrawArc(bisector->point1, bisector->length, bisector->initial_angle, 1.0f, bisector->progress, bisector-> thickness, bisector->arcColour, bisector->lineColour);
            break;

        case ARC1_WIDDERSHINS:

            // includes conversion from radians to degrees, why does it use degrees??? am i missing something???
            DrawRing(bisector->point1, bisector->length, bisector->length + bisector->thickness, (360 * bisector->initial_angle) / (2 * PI), (360 * bisector->initial_angle) / (2 * PI) + 72, 100, bisector->arcColour);
            DrawArc(bisector->point1, bisector->length, bisector->initial_angle + 0.4f * PI, -1.0f, bisector->progress, bisector-> thickness, bisector->arcColour, bisector->lineColour);
            break;

        case ARC2_WIDDERSHINS:

            DrawRing(bisector->point1, bisector->length, bisector->length + bisector->thickness, (360 * bisector->initial_angle) / (2 * PI), (360 * bisector->initial_angle) / (2 * PI) + 72, 100, bisector->arcColour);
            DrawArc(bisector->point1, bisector->length, bisector->initial_angle, -1.0f, bisector->progress, bisector-> thickness, bisector->arcColour, bisector->lineColour);
            break;

        case ARC2_CLOCKWISE:

            DrawRing(bisector->point1, bisector->length, bisector->length + bisector->thickness, (360 * bisector->initial_angle) / (2 * PI) + 72, (360 * bisector->initial_angle) / (2 * PI) - 72, 100, bisector->arcColour);
            DrawArc(bisector->point1, bisector->length, bisector->initial_angle - 0.4f * PI, 1.0f, bisector->progress, bisector-> thickness, bisector->arcColour, bisector->lineColour);
            break;

        case ARC3_CLOCKWISE:

            DrawRing(bisector->point1, bisector->length, bisector->length + bisector->thickness, (360 * bisector->initial_angle) / (2 * PI) + 72, (360 * bisector->initial_angle) / (2 * PI) - 72, 100, bisector->arcColour);
            DrawArc(bisector->point2, bisector->length, bisector->initial_angle + PI, 1.0f, bisector->progress, bisector-> thickness, bisector->arcColour, bisector->lineColour);
            break;

        case ARC3_WIDDERSHINS:

            DrawRing(bisector->point1, bisector->length, bisector->length + bisector->thickness, (360 * bisector->initial_angle) / (2 * PI) + 72, (360 * bisector->initial_angle) / (2 * PI) - 72, 100, bisector->arcColour);
            DrawRing(bisector->point2, bisector->length, bisector->length + bisector->thickness, (360 * bisector->initial_angle) / (2 * PI) + 180, (360 * bisector->initial_angle) / (2 * PI) + 252, 100, bisector->arcColour);
            DrawArc(bisector->point2, bisector->length, bisector->initial_angle - 0.6 * PI, -1.0f, bisector->progress, bisector-> thickness, bisector->arcColour, bisector->lineColour);
            break;

        case ARC4_CLOCKWISE:

            DrawRing(bisector->point1, bisector->length, bisector->length + bisector->thickness, (360 * bisector->initial_angle) / (2 * PI) + 72, (360 * bisector->initial_angle) / (2 * PI) - 72, 100, bisector->arcColour);
            DrawRing(bisector->point2, bisector->length, bisector->length + bisector->thickness, (360 * bisector->initial_angle) / (2 * PI) + 180, (360 * bisector->initial_angle) / (2 * PI) + 252, 100, bisector->arcColour);
            DrawArc(bisector->point2, bisector->length, bisector->initial_angle + PI, -1.0f, bisector->progress, bisector-> thickness, bisector->arcColour, bisector->lineColour);
            break;

        case ARC4_WIDDERSHINS:

            DrawRing(bisector->point1, bisector->length, bisector->length + bisector->thickness, (360 * bisector->initial_angle) / (2 * PI) + 72, (360 * bisector->initial_angle) / (2 * PI) - 72, 100, bisector->arcColour);
            DrawRing(bisector->point2, bisector->length, bisector->length + bisector->thickness, (360 * bisector->initial_angle) / (2 * PI) + 252, (360 * bisector->initial_angle) / (2 * PI) + 108, 100, bisector->arcColour);
            DrawArc(bisector->point2, bisector->length, bisector->initial_angle + 0.6 * PI, 1.0f, bisector->progress, bisector-> thickness, bisector->arcColour, bisector->lineColour);
            break;

        case BISECTION:

            DrawRing(bisector->point1, bisector->length, bisector->length + bisector->thickness, (360 * bisector->initial_angle) / (2 * PI) + 72, (360 * bisector->initial_angle) / (2 * PI) - 72, 100, bisector->arcColour);
            DrawRing(bisector->point2, bisector->length, bisector->length + bisector->thickness, (360 * bisector->initial_angle) / (2 * PI) + 252, (360 * bisector->initial_angle) / (2 * PI) + 108, 100, bisector->arcColour);
            DrawLineEx(bisector->point1, bisector->point2, bisector->thickness, bisector->lineColour);

            DrawBisectingLine(bisector->point1, bisector->point2, bisector->length, bisector->bisector_angle, bisector->progress, bisector->thickness, bisector->lineColour);

            break;

        case BISECTORCOMPLETE:

            DrawRing(bisector->point1, bisector->length, bisector->length + bisector->thickness, (360 * bisector->initial_angle) / (2 * PI) + 72 , (360 * bisector->initial_angle) / (2 * PI) - 72, 100, bisector->arcColour);
            DrawRing(bisector->point2, bisector->length, bisector->length + bisector->thickness, (360 * bisector->initial_angle) / (2 * PI) + 252, (360 * bisector->initial_angle) / (2 * PI) + 108, 100, bisector->arcColour);
            DrawLineEx(bisector->point1, bisector->point2, bisector->thickness, bisector->lineColour);

            DrawBisectingLine(bisector->point1, bisector->point2, bisector->length, bisector->bisector_angle, 1.0f, bisector->thickness, bisector->lineColour);

            break;
    }
}