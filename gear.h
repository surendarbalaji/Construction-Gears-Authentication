#pragma once
#include "raylib.h"
#include "constructions/constructions.h"

typedef struct {
    Vector2 position;
    Vector2 currentPointA;
    Vector2 currentPointB;
    int number;
    float angle;
    ColourConfiguration colours;

    Construction instructions[32];
    int instruction_count;
    int active_instruction;
    int processedCount; // i know i've switched from underscores but oh well
} Gear;

void AddInstruction(Gear* gear, ConstructionType type);
void UpdateGear(Gear* gear, float dt);
void DrawGear(Gear* gear);