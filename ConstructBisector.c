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

        // arc construction is split into nine segments for now, 8 x 0.5 for each half arc and 1.0 for the final line
        if (bisector->progress >= 5.0) {
            bisector->progress = 5.0;
            bisector->complete = true;
        }
    }
}

void DrawBisectorConstruction(const BisectorConstructor* bisector) {
    const int segments = 100;


    // i really thought this was a perfect switch 'case', if you know what i mean, but of course it turns out switch can only handle integers
    // oh well, back to simple elifs, this just guides through each stage of arc construction and arm rotation, there sure is a lot of repeated code though (there is definitely a better way to do this)

    // oh wow, this is quite possibly the worst thing i've ever written. is it the absolute worst solution to this problem? maybe. i'll one hundred percent fix it later im just so tired of constructing bisectors

    if (bisector->progress < 0.5) {
        float step = (0.8f * PI * bisector->progress) / segments;

        for (int i = 0; i < segments; i++) {

            // arc of circle point1 to point2
            float angle1 = (bisector->initial_angle) + i * step;
            float angle2 = (bisector->initial_angle) + (i + 1) * step;

            Vector2 p1 = {
                bisector->point1.x + bisector->length * cosf(angle1),
                bisector->point1.y + bisector->length * sinf(angle1)
            };
            Vector2 p2 = {
                bisector->point1.x + bisector->length * cosf(angle2),
                bisector->point1.y + bisector->length * sinf(angle2)
            };

            DrawLineEx(p1, p2, bisector->thickness, bisector->arcColour);
        }

        float arm_angle = bisector->initial_angle + (0.8f * PI * bisector->progress);
        Vector2 arm_end = {
            bisector->point1.x + bisector->length * cosf(arm_angle),
            bisector->point1.y + bisector->length * sinf(arm_angle)
        };
        DrawLineEx(bisector->point1, arm_end, bisector->thickness, bisector->lineColour);

    } else if ( bisector->progress >= 0.5 && bisector->progress < 1.0) {
        float step = (0.4f * PI) / segments;

        for (int i = 0; i < segments; i++) {
            float angle1 = bisector->initial_angle + i * step;
            float angle2 = bisector->initial_angle + (i + 1) * step;

            Vector2 p1 = {
                bisector->point1.x + bisector->length * cosf(angle1),
                bisector->point1.y + bisector->length * sinf(angle1)
            };
            Vector2 p2 = {
                bisector->point1.x + bisector->length * cosf(angle2),
                bisector->point1.y + bisector->length * sinf(angle2)
            };

            DrawLineEx(p1, p2, bisector->thickness, bisector->arcColour);
        }

        float arm_angle = bisector->initial_angle + 0.4f * PI - (0.8f * PI * (bisector->progress - 0.5));
        Vector2 arm_end = {
            bisector->point1.x + bisector->length * cosf(arm_angle),
            bisector->point1.y + bisector->length * sinf(arm_angle)
        };
        DrawLineEx(bisector->point1, arm_end, bisector->thickness, bisector->lineColour);

    } else if ( bisector->progress >= 1.0 && bisector->progress < 1.5) {
        float step = (-0.8f * PI) * (bisector->progress - 0.5) / segments;

        for (int i = 0; i < segments; i++) {
            float angle1 = bisector->initial_angle + (0.4f * PI) + i * step;
            float angle2 = bisector->initial_angle + (0.4f * PI) + (i + 1) * step;

            Vector2 p1 = {
                bisector->point1.x + bisector->length * cosf(angle1),
                bisector->point1.y + bisector->length * sinf(angle1)
            };
            Vector2 p2 = {
                bisector->point1.x + bisector->length * cosf(angle2),
                bisector->point1.y + bisector->length * sinf(angle2)
            };

            DrawLineEx(p1, p2, bisector->thickness, bisector->arcColour);
        }

        float arm_angle = bisector->initial_angle - (0.8f * PI * (bisector->progress - 1.0));
        Vector2 arm_end = {
            bisector->point1.x + bisector->length * cosf(arm_angle),
            bisector->point1.y + bisector->length * sinf(arm_angle)
        };
        DrawLineEx(bisector->point1, arm_end, bisector->thickness, bisector->lineColour);

    } else if ( bisector->progress >= 1.5 && bisector->progress < 2.0) {
        float step = (-0.8f * PI)  / segments;

        for (int i = 0; i < segments; i++) {
            float angle1 = bisector->initial_angle + (0.4f * PI) + i * step;
            float angle2 = bisector->initial_angle + (0.4f * PI) + (i + 1) * step;

            Vector2 p1 = {
                bisector->point1.x + bisector->length * cosf(angle1),
                bisector->point1.y + bisector->length * sinf(angle1)
            };
            Vector2 p2 = {
                bisector->point1.x + bisector->length * cosf(angle2),
                bisector->point1.y + bisector->length * sinf(angle2)
            };

            DrawLineEx(p1, p2, bisector->thickness, bisector->arcColour);
        }

        float arm_angle = bisector->initial_angle - 0.4f * PI + (0.8f * PI * (bisector->progress - 1.5));
        Vector2 arm_end = {
            bisector->point1.x + bisector->length * cosf(arm_angle),
            bisector->point1.y + bisector->length * sinf(arm_angle)
        };
        DrawLineEx(bisector->point1, arm_end, bisector->thickness, bisector->lineColour);

    } else if ( bisector->progress >= 2.0 && bisector->progress < 2.5) {
        float step = (-0.8f * PI)  / segments;
        float step2 =  0.8 * PI * (bisector->progress - 2.0f) / segments; // hah its step two, like the exam, the exam for cambridge, the one upon which some offers are conditional, you know?

        for (int i = 0; i < segments; i++) {
            float angle1 = bisector->initial_angle + (0.4f * PI) + i * step;
            float angle2 = bisector->initial_angle + (0.4f * PI) + (i + 1) * step;

            float angle3 = bisector->initial_angle + i * step2;
            float angle4 = bisector->initial_angle + (i + 1) * step2;

            Vector2 p1 = {
                bisector->point1.x + bisector->length * cosf(angle1),
                bisector->point1.y + bisector->length * sinf(angle1)
            };
            Vector2 p2 = {
                bisector->point1.x + bisector->length * cosf(angle2),
                bisector->point1.y + bisector->length * sinf(angle2)
            };

            Vector2 p3 = {
                bisector->point2.x + bisector->length * -cosf(angle3),
                bisector->point2.y + bisector->length * -sinf(angle3)
            };
            Vector2 p4 = {
                bisector->point2.x + bisector->length * -cosf(angle4),
                bisector->point2.y + bisector->length * -sinf(angle4)
            };

            DrawLineEx(p1, p2, bisector->thickness, bisector->arcColour);
            DrawLineEx(p3, p4, bisector->thickness, bisector->arcColour);
        }

        float arm_angle = bisector->initial_angle + (0.8f * PI * (bisector->progress - 2.0));
        Vector2 arm_end = {
            bisector->point2.x - bisector->length * cosf(arm_angle),
            bisector->point2.y - bisector->length * sinf(arm_angle)
        };
        DrawLineEx(bisector->point2, arm_end, bisector->thickness, bisector->lineColour);

    } else if ( bisector->progress >= 2.5 && bisector->progress < 3.0) {
        float step = (-0.8f * PI)  / segments;
        float step2 = 0.4f * PI / segments;

        for (int i = 0; i < segments; i++) {
            float angle1 = bisector->initial_angle + (0.4f * PI) + i * step;
            float angle2 = bisector->initial_angle + (0.4f * PI) + (i + 1) * step;

            float angle3 = bisector->initial_angle + i * step2;
            float angle4 = bisector->initial_angle + (i + 1) * step2;

            Vector2 p1 = {
                bisector->point1.x + bisector->length * cosf(angle1),
                bisector->point1.y + bisector->length * sinf(angle1)
            };
            Vector2 p2 = {
                bisector->point1.x + bisector->length * cosf(angle2),
                bisector->point1.y + bisector->length * sinf(angle2)
            };

            Vector2 p3 = {
                bisector->point2.x + bisector->length * -cosf(angle3),
                bisector->point2.y + bisector->length * -sinf(angle3)
            };
            Vector2 p4 = {
                bisector->point2.x + bisector->length * -cosf(angle4),
                bisector->point2.y + bisector->length * -sinf(angle4)
            };

            DrawLineEx(p1, p2, bisector->thickness, bisector->arcColour);
            DrawLineEx(p3, p4, bisector->thickness, bisector->arcColour);
        }

        float arm_angle = bisector->initial_angle + 0.4f * PI - (0.8f * PI * (bisector->progress - 2.5));
        Vector2 arm_end = {
            bisector->point2.x - bisector->length * cosf(arm_angle),
            bisector->point2.y - bisector->length * sinf(arm_angle)
        };
        DrawLineEx(bisector->point2, arm_end, bisector->thickness, bisector->lineColour);

    } else if ( bisector->progress >= 3.0 && bisector->progress < 3.5) {
        float step = (-0.8f * PI)  / segments;
        float step2 = (-0.8f * PI) * (bisector->progress - 2.5) / segments;

        for (int i = 0; i < segments; i++) {
            float angle1 = bisector->initial_angle + (0.4f * PI) + i * step;
            float angle2 = bisector->initial_angle + (0.4f * PI) + (i + 1) * step;

            float angle3 = bisector->initial_angle + (0.4f * PI)  + i * step2;
            float angle4 = bisector->initial_angle + (0.4f * PI) + (i + 1) * step2;

            Vector2 p1 = {
                bisector->point1.x + bisector->length * cosf(angle1),
                bisector->point1.y + bisector->length * sinf(angle1)
            };
            Vector2 p2 = {
                bisector->point1.x + bisector->length * cosf(angle2),
                bisector->point1.y + bisector->length * sinf(angle2)
            };

            Vector2 p3 = {
                bisector->point2.x + bisector->length * -cosf(angle3),
                bisector->point2.y + bisector->length * -sinf(angle3)
            };
            Vector2 p4 = {
                bisector->point2.x + bisector->length * -cosf(angle4),
                bisector->point2.y + bisector->length * -sinf(angle4)
            };

            DrawLineEx(p1, p2, bisector->thickness, bisector->arcColour);
            DrawLineEx(p3, p4, bisector->thickness, bisector->arcColour);
        }

        float arm_angle = bisector->initial_angle - (0.8f * PI * (bisector->progress - 3.0));
        Vector2 arm_end = {
            bisector->point2.x - bisector->length * cosf(arm_angle),
            bisector->point2.y - bisector->length * sinf(arm_angle)
        };
        DrawLineEx(bisector->point2, arm_end, bisector->thickness, bisector->lineColour);

    } else if ( bisector->progress >= 3.5 && bisector->progress < 4.0) {
        float step = (-0.8f * PI)  / segments;
        float step2 = (-0.8f * PI) / segments;

        for (int i = 0; i < segments; i++) {
            float angle1 = bisector->initial_angle + (0.4f * PI) + i * step;
            float angle2 = bisector->initial_angle + (0.4f * PI) + (i + 1) * step;

            float angle3 = bisector->initial_angle + (0.4f * PI)  + i * step2;
            float angle4 = bisector->initial_angle + (0.4f * PI) + (i + 1) * step2;

            Vector2 p1 = {
                bisector->point1.x + bisector->length * cosf(angle1),
                bisector->point1.y + bisector->length * sinf(angle1)
            };
            Vector2 p2 = {
                bisector->point1.x + bisector->length * cosf(angle2),
                bisector->point1.y + bisector->length * sinf(angle2)
            };

            Vector2 p3 = {
                bisector->point2.x + bisector->length * -cosf(angle3),
                bisector->point2.y + bisector->length * -sinf(angle3)
            };
            Vector2 p4 = {
                bisector->point2.x + bisector->length * -cosf(angle4),
                bisector->point2.y + bisector->length * -sinf(angle4)
            };

            DrawLineEx(p1, p2, bisector->thickness, bisector->arcColour);
            DrawLineEx(p3, p4, bisector->thickness, bisector->arcColour);
        }

        float arm_angle = bisector->initial_angle - 0.4f * PI + (0.8f * PI * (bisector->progress - 3.5));
        Vector2 arm_end = {
            bisector->point2.x - bisector->length * cosf(arm_angle),
            bisector->point2.y - bisector->length * sinf(arm_angle)
        };
        DrawLineEx(bisector->point2, arm_end, bisector->thickness, bisector->lineColour);

    } else {
        float step = (-0.8f * PI)  / segments;
        float step2 = (-0.8f * PI) / segments;

        for (int i = 0; i < segments; i++) {
            float angle1 = bisector->initial_angle + (0.4f * PI) + i * step;
            float angle2 = bisector->initial_angle + (0.4f * PI) + (i + 1) * step;

            float angle3 = bisector->initial_angle + (0.4f * PI)  + i * step2;
            float angle4 = bisector->initial_angle + (0.4f * PI) + (i + 1) * step2;

            Vector2 p1 = {
                bisector->point1.x + bisector->length * cosf(angle1),
                bisector->point1.y + bisector->length * sinf(angle1)
            };
            Vector2 p2 = {
                bisector->point1.x + bisector->length * cosf(angle2),
                bisector->point1.y + bisector->length * sinf(angle2)
            };

            Vector2 p3 = {
                bisector->point2.x + bisector->length * -cosf(angle3),
                bisector->point2.y + bisector->length * -sinf(angle3)
            };
            Vector2 p4 = {
                bisector->point2.x + bisector->length * -cosf(angle4),
                bisector->point2.y + bisector->length * -sinf(angle4)
            };

            DrawLineEx(p1, p2, bisector->thickness, bisector->arcColour);
            DrawLineEx(p3, p4, bisector->thickness, bisector->arcColour);
        }

        DrawLineEx(bisector->point1, bisector->point2, bisector->thickness, bisector->lineColour);

        // TRIGONOMETRY !!!!! (drawing perpendicular bisector itself)
        // we are finding the length of the new bisector line by multiplying length between our two points by sin pi/3 (this angle is constant since the arcs have radius equal to length between points)
        // this is multiplied the sine and cosine of the bisector angle to find the change in x and y respectively, then finally added to the midpoint
        Vector2 bisectorPoint = {
            (bisector->point1.x + bisector->point2.x) / 2 + ((bisector->progress - 4.0) * bisector->length * sinf(PI/3) * cosf(bisector->bisector_angle) / (0.2 * PI)), // i don't want to recalculate the amount by which I should divide so i've just set it to 0.2pi for now
            (bisector->point1.y + bisector->point2.y) / 2 + ((bisector->progress - 4.0) * bisector->length * sinf(PI/3) * sinf(bisector->bisector_angle) / (0.2 * PI))
        };

        // same thing, opposite direction
        Vector2 bisectorPoint2 = {
            (bisector->point1.x + bisector->point2.x) / 2 - ((bisector->progress - 4.0) * bisector->length * sinf(PI/3) * cosf(bisector->bisector_angle) / (0.2 * PI)),
            (bisector->point1.y + bisector->point2.y) / 2 - ((bisector->progress - 4.0) * bisector->length * sinf(PI/3) * sinf(bisector->bisector_angle) / (0.2 * PI))
        };

        DrawLineEx(bisectorPoint, bisectorPoint2, bisector->thickness, ORANGE);

    }
}