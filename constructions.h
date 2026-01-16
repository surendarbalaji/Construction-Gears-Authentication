#pragma once
#include "raylib.h"

typedef struct {
    Vector2 centre;
    Vector2 point;
    float progress;
    float radius;
    float speed;
    float thickness;
    float initial_angle;
    Color circleColor;
    Color radiusColor;
    bool complete;
} CircleConstructor;

CircleConstructor InitialiseCircle(Vector2 centre, Vector2 point, float speed, float thickness, Color circleColor, Color radiusColor);
void UpdateCircle(CircleConstructor* circle, float dt);
void DrawCircleConstruction(const CircleConstructor* circle);