#pragma once
#include "raylib.h"

// circle construction tools

typedef struct {
    Vector2 centre;
    Vector2 point;
    float progress;
    float radius;
    float speed;
    float thickness;
    float initial_angle;
    Color circleColour;
    Color radiusColour;
    bool complete;
} CircleConstructor;

CircleConstructor InitialiseCircle(Vector2 centre, Vector2 point, float speed, float thickness, Color circleColour, Color radiusColour); // if c doesn't have classes is the solution to just pretend it does? (i wish i had attended one)
void UpdateCircle(CircleConstructor* circle, float dt); // takes pointer as so to update the original struct
void DrawCircleConstruction(const CircleConstructor* circle); // takes updated struct data as a const since values are calculated by the previous function


// perpendicular bisector construction tools

typedef struct {
    Vector2 point1;
    Vector2 point2;
    float progress;
    float length;
    float speed;
    float thickness;
    float initial_angle;
    float bisector_angle;
    Color arcColour;
    Color lineColour;
    bool complete;
} BisectorConstructor;

BisectorConstructor InitialiseBisector(Vector2 point1, Vector2 point2, float speed, float thickness, Color arcColour, Color lineColour);
void UpdateBisector(BisectorConstructor* bisector, float dt);
void DrawBisectorConstruction(const BisectorConstructor* bisector);
