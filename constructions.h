#pragma once
#include "raylib.h"
#include <stdint.h>

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

// using an enum to cycle through phases of construction

typedef enum {
    ARC1_CLOCKWISE,
    ARC1_WIDDERSHINS,
    ARC2_CLOCKWISE,
    ARC2_WIDDERSHINS,
    ARC3_CLOCKWISE,
    ARC3_WIDDERSHINS,
    ARC4_CLOCKWISE,
    ARC4_WIDDERSHINS,
    BISECTION,
    BISECTORCOMPLETE
} BisectorPhase;

typedef struct {
    Vector2 pointA;
    Vector2 pointB;
    float progress;
    float length;
    float speed;
    float thickness;
    float initial_angle;
    float bisector_angle;
    Color arcColour;
    Color lineColour;
    BisectorPhase phase;
} BisectorConstructor;

BisectorConstructor InitialiseBisector(Vector2 pointA, Vector2 pointB, float speed, float thickness, Color arcColour, Color lineColour);
void UpdateBisector(BisectorConstructor* bisector, float dt);
void DrawBisectorConstruction(const BisectorConstructor* bisector);


// equilateral triangle construction tools

typedef enum {
    CIRCLES,
    SIDES,
    EQUILATERALCOMPLETE
} EquilateralPhase;

typedef struct {
    Vector2 pointA;
    Vector2 pointB;
    Vector2 pointC;
    Vector2 faceAwayFrom;
    float progress;
    float length;
    float speed;
    float thickness;
    float initial_angle;
    Color arcColour;
    Color lineColour;
    EquilateralPhase phase;
    uint8_t hideMask;

    CircleConstructor circleA;
    CircleConstructor circleB;

} EquilateralConstructor;

#define EQUILATERAL_SHOW 0 // using a bitmask to be able to hide lines
#define EQUILATERAL_HIDEAB (1 << 0) // is it legal to put definitions half way down the file?
#define EQUILATERAL_HIDEAC (1 << 1)
#define EQUILATERAL_HIDEBC (1 << 2)

EquilateralConstructor InitialiseEquilateral(Vector2 pointA, Vector2 pointB, Vector2 faceAwayFrom, float speed, float thickness, Color arcColour, Color lineColour);
void UpdateEquilateral(EquilateralConstructor* equilateral, float dt);
void DrawEquilateralConstruction(const EquilateralConstructor* equilateral);


// angle bisector construction tools

typedef enum {
    CIRCLEAD,
    LINEDE,
    EQUILATERALED,
    LINEAF,
    ANGLEBISECTORCOMPLETE
} AngleBisectorPhase;

typedef struct {
    Vector2 pointB; // B first because I like working with angle BAC
    Vector2 pointA;
    Vector2 pointC;
    Vector2 pointD;
    Vector2 pointE;
    float progress;
    float length;
    float speed;
    float thickness;
    float angle_BA;
    float angle_EA;
    Color lengthColour;
    Color arcColour;
    Color equilateralLineColour;
    Color equilateralArcColour;
    AngleBisectorPhase phase;

    CircleConstructor circleAD;
    EquilateralConstructor equilateralED;

} AngleBisectorConstructor;

AngleBisectorConstructor InitialiseAngleBisector(Vector2 pointB, Vector2 pointA, Vector2 pointC, float speed, float thickness, Color lengthColour, Color arcColour, Color equilateralLineColour, Color equilateralArcColour);
void UpdateAngleBisector(AngleBisectorConstructor* angleBisector, float dt);
void DrawAngleBisectorConstruction(const AngleBisectorConstructor* angleBisector);


// equal line construction tools

typedef enum {
    LINEBC,
    EQUILATERAL,
    CIRCLEBA,
    EXTENDDB,
    CIRCLEDE,
    EXTENDDC,
    EQUALLINECOMPLETE
} EqualLinePhase;

typedef struct {
    Vector2 pointA;
    Vector2 pointB;
    Vector2 pointC;
    Vector2 pointE;
    Vector2 pointF;
    float progress;
    float length;
    float speed;
    float thickness;
    float angle_BE;
    float angle_CF;
    Color lengthColour;
    Color arcColour;
    Color equilateralLineColour;
    Color equilateralArcColour;
    EqualLinePhase phase;

    EquilateralConstructor equilateralBC;
    CircleConstructor circleBA;
    CircleConstructor circleDE;

} EqualLineConstructor;

EqualLineConstructor InitialiseEqualLine(Vector2 pointA, Vector2 pointB, Vector2 pointC, float speed, float thickness, Color lengthColour, Color arcColour, Color equilateralLineColour, Color equilateralArcColour);
void UpdateEqualLine(EqualLineConstructor* equalLine, float dt);
void DrawEqualLineConstruction(const EqualLineConstructor* equalLine);



