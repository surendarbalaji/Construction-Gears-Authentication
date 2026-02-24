#pragma once
#include "raylib.h"
#include "constructions/constructions.h"

typedef struct {
    Vector2 position;
    Vector2 currentPointA;
    Vector2 currentPointB;
    float angle;
    ColourConfiguration colours;

    Construction instructions[8];
    int instruction_count;
    int active_instruction;
} Gear;

void AddInstruction(Gear* gear, ConstructionType type);
void UpdateGear(Gear* gear, float dt);
void DrawGear(Gear* gear);