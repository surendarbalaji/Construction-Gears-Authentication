#include "constructions/constructions.h"

// fading function to dim used elements

void FadeConstruction(ColourConfiguration* colours, float rate, float min_alpha, float dt) {

    colours->alpha -= rate * dt;
    if (colours->alpha < min_alpha) colours->alpha = min_alpha;

}
