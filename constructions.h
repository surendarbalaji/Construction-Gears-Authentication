#pragma once
#include "raylib.h"
#include <stdint.h>

// colour configuration struct to manegably configure colours while having a default for each construction

typedef struct {
    Color line1;
    Color line2;
    Color line3;
    Color arc1;
    Color arc2;
    Color arc3;
    float alpha;
} ColourConfiguration;

extern ColourConfiguration SelectedColours;

// fading function to dim used elements

void FadeConstruction(ColourConfiguration* colours, float rate, float min_alpha, float dt);


// circle construction I.Postulate 3

typedef struct {
    Vector2 centre;
    Vector2 point;
    float progress;
    float radius;
    float speed;
    float thickness;
    float initial_angle;
    ColourConfiguration colours;
    uint8_t hideMask;
    bool complete;
} CircleConstructor;

#define CIRCLE_SHOW 0
#define CIRCLE_HIDELINE (1 << 0)
#define CIRCLE_HIDEARC (1 << 1)


CircleConstructor InitialiseCircle(Vector2 centre, Vector2 point, float speed, float thickness, ColourConfiguration SelectedColours); // if c doesn't have classes is the solution to just pretend it does? (i wish i had attended one)
void UpdateCircle(CircleConstructor* circle, float dt); // takes pointer as so to update the original struct
void DrawCircleConstruction(const CircleConstructor* circle); // takes updated struct data as a const since values are calculated by the previous function


// equilateral triangle construction I.i

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
    ColourConfiguration colours;
    EquilateralPhase phase;
    uint8_t hideMask;

    CircleConstructor circleA;
    CircleConstructor circleB;

} EquilateralConstructor;

extern ColourConfiguration equilateralColours;

#define EQUILATERAL_SHOW 0 // using a bitmask to be able to hide lines
#define EQUILATERAL_HIDEAB (1 << 0) // is it legal to put definitions half way down the file?
#define EQUILATERAL_HIDEAC (1 << 1)
#define EQUILATERAL_HIDEBC (1 << 2)

EquilateralConstructor InitialiseEquilateral(Vector2 pointA, Vector2 pointB, Vector2 faceAwayFrom, float speed, float thickness, ColourConfiguration equilateralColours);
void UpdateEquilateral(EquilateralConstructor* equilateral, float dt);
void DrawEquilateralConstruction(const EquilateralConstructor* equilateral);


// equal line construction I.ii

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
    ColourConfiguration colours;
    EqualLinePhase phase;
    uint8_t hideMask;

    EquilateralConstructor equilateralBC;
    CircleConstructor circleBA;
    CircleConstructor circleDE;

} EqualLineConstructor;

extern ColourConfiguration equalLineColours;

#define EQUALLINE_SHOW 0
#define EQUALLINE_HIDEDC (1 << 0)

EqualLineConstructor InitialiseEqualLine(Vector2 pointA, Vector2 pointB, Vector2 pointC, float speed, float thickness, ColourConfiguration equalLineColours);
void UpdateEqualLine(EqualLineConstructor* equalLine, float dt);
void DrawEqualLineConstruction(const EqualLineConstructor* equalLine);


// equal length cut construction I.iii

typedef enum {
    EQUALLINE,
    CIRCLECE,
    LINECF,
    EQUALLENGTHCUTCOMPLETE
} EqualLengthCutPhase;

typedef struct {
    Vector2 pointA;
    Vector2 pointB;
    Vector2 pointC;
    Vector2 pointD;
    Vector2 pointF;
    float progress;
    float length;
    float speed;
    float thickness;
    float angle_CD;
    ColourConfiguration colours;
    EqualLengthCutPhase phase;

    EqualLineConstructor equalLineAB;
    CircleConstructor circleCF;

} EqualLengthCutConstructor;

extern ColourConfiguration equalLengthCutColours;

EqualLengthCutConstructor InitialiseEqualLengthCut(Vector2 pointA, Vector2 pointB, Vector2 pointC, Vector2 pointD, float speed, float thickness, ColourConfiguration equalLengthCutColours);
void UpdateEqualLengthCut(EqualLengthCutConstructor* equalLengthCut, float dt);
void DrawEqualLengthCutConstruction(const EqualLengthCutConstructor* equalLengthCut);


// angle bisector construction I.ix

typedef enum {
    CIRCLEAD,
    AB_LINEDE,
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
    ColourConfiguration colours;
    AngleBisectorPhase phase;

    CircleConstructor circleAD;
    EquilateralConstructor equilateralED;

} AngleBisectorConstructor;

extern ColourConfiguration angleBisectorColours;

AngleBisectorConstructor InitialiseAngleBisector(Vector2 pointB, Vector2 pointA, Vector2 pointC, float speed, float thickness, ColourConfiguration angleBisectorColours);
void UpdateAngleBisector(AngleBisectorConstructor* angleBisector, float dt);
void DrawAngleBisectorConstruction(const AngleBisectorConstructor* angleBisector);


// perpendicular construction 1.xi

typedef enum {
    CIRCLEAC,
    EQUILATERALDE,
    LINEFC,
    PERPENDICULARCOMPLETE
} PerpendicularPhase;

typedef struct {
    Vector2 pointA;
    Vector2 pointB;
    Vector2 pointC;
    Vector2 pointD;
    Vector2 pointE;
    float progress;
    float length;
    float speed;
    float thickness;
    float angle_AB;
    ColourConfiguration colours;
    PerpendicularPhase phase;

    CircleConstructor circleAC;
    EquilateralConstructor equilateralED;

} PerpendicularConstructor;

extern ColourConfiguration perpendicularColours;

PerpendicularConstructor InitialisePerpendicular(Vector2 pointA, Vector2 pointB, Vector2 pointC, float speed, float thickness, ColourConfiguration perpendicularColours);
void UpdatePerpendicular(PerpendicularConstructor* perpendicular, float dt);
void DrawPerpendicularConstruction(const PerpendicularConstructor* angleBisector);


// Equal Angle Construction I.xxiii

typedef enum {
    EA_LINEDE,
    EQUALLENGTHCUTCE,
    EQUALLINECD,
    CIRCLEAG,
    EQUALLINEDE,
    CIRCLEFG,
    TRIANGLEAGF,
    EQUALANGLECOMPLETE
} EqualAnglePhase;

typedef struct {
    Vector2 pointA;
    Vector2 pointB;
    Vector2 pointC2;
    Vector2 pointC1; // angle on C1
    Vector2 pointC3;
    Vector2 pointD;
    Vector2 pointE;
    Vector2 pointF;
    Vector2 pointG;
    float progress;
    float speed;
    float thickness;
    float angle_C;
    float angle_G;
    float angle_CD;
    float angle_CE;
    ColourConfiguration colours;
    EqualAnglePhase phase;
    uint8_t hideMask;

    EqualLengthCutConstructor equalLengthCutCE;
    EqualLineConstructor equalLineCD;
    CircleConstructor circleAG;
    EqualLineConstructor equalLineDE;
    CircleConstructor circleFG;

} EqualAngleConstructor;

#define EQUALANGLE_SHOW 0
#define EQUALANGLE_HIDEC1D (1 << 0)
#define EQUALANGLE_HIDEDE (1 << 1)
#define EQUALANGLE_HIDEAG (1 << 2)
#define EQUALANGLE_HIDEFG (1 << 3)

extern ColourConfiguration equalAngleColours;

EqualAngleConstructor InitialiseEqualAngle(Vector2 pointA, Vector2 pointB, Vector2 pointC2, Vector2 pointC1, Vector2 pointC3, float speed, float thickness, ColourConfiguration equalAngleColours);
void UpdateEqualAngle(EqualAngleConstructor* equalAngle, float dt);
void DrawEqualAngleConstruction(const EqualAngleConstructor* equalAngle);


// parallel line construction I.xxxi

typedef enum {
    LINEAD,
    EQUALANGLE,
    P_LINEAF,
    PARALLELCOMPLETE
} ParallelPhase;

typedef struct {
    Vector2 pointA;
    Vector2 pointB;
    Vector2 pointC;
    Vector2 pointD;
    Vector2 pointF1;
    Vector2 pointF2;
    float progress;
    float length;
    float speed;
    float thickness;
    float angle_G;
    ColourConfiguration colours;
    ParallelPhase phase;

    EqualAngleConstructor equalAngleADC;

} ParallelConstructor;

extern ColourConfiguration parallelColours;

ParallelConstructor InitialiseParallel(Vector2 pointA, Vector2 pointB, Vector2 pointC, float speed, float thickness, ColourConfiguration parallelColours);
void UpdateParallel(ParallelConstructor* parallel, float dt);
void DrawParallelConstruction(const ParallelConstructor* parallel);


// perpendicular bisector construction - written when I was testing things out, the actual construction doesn't operate exactly like this

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



