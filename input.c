#include <stdio.h>

#include "input.h"
#include "gear.h"



char input[32] = "\0";
int inputValue[32];
int letterCount = 0;


// stolen from my relative simulator project
void takeInput() {

    int key = GetCharPressed();

    while (key > 0) {

        if ((key >= 32) && (key <= 125)) {

            if (letterCount < 32) {

                input[letterCount] = (char)key;
                input[letterCount+1] = '\0';
                inputValue[letterCount] = key;
                letterCount++;

            }

        key = GetCharPressed();

        }

    }

    // if (IsKeyPressedRepeat(KEY_BACKSPACE) || IsKeyPressed(KEY_BACKSPACE)) {
    //     letterCount--;
    //     if (letterCount < 0) letterCount = 0;
    //     input[letterCount] = '\0';
    //     if (processedCount > letterCount) processedCount = letterCount;
    // }

}

void processInput(Gear* gear) {

    if (gear->processedCount >= letterCount) return;

    if (gear->active_instruction >= gear->instruction_count) {

        int nextValue = inputValue[gear->processedCount];

        // // default to circle
        // ConstructionType type = CIRCLE;
        //
        // if (nextChar == 'C' || nextChar == 'c') {
        //     type = CIRCLE;
        // } else if (nextChar == 'E' || nextChar == 'e') {
        //     type = EQUILATERAL;
        // } else if (nextChar == 'B' || nextChar == 'b') {
        //     type = PERPENDICULAR_BISECTOR;
        // } else if (nextChar == 'P' || nextChar == 'p') {
        //     type = PERPENDICULAR;
        // }

        int instructionKey = (nextValue + gear->number) % 4;
        ConstructionType type = CIRCLE;

        switch (instructionKey) {

            case 0:
                type = CIRCLE;
                break;

            case 1:
                type = EQUILATERAL;
                break;

            case 2:
                type = PERPENDICULAR_BISECTOR;
                break;

            case 3:
                type = PERPENDICULAR;
                break;
        }

        AddInstruction(gear, type);
        gear->processedCount++;
    }

}

