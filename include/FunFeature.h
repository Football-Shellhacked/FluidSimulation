#ifndef FUNFEATURE_H
#define FUNFEATURE_H
#include "raylib.h"

class FunFeatures{

    public:
        static bool cursorInteraction;
        static bool useGravity;
        static bool DrawCheckboxWithLabel(Rectangle bounds, bool checked, const char* label, Color color);
        static float DrawSlider(Rectangle slider, float minValue, float maxValue, float value, Color color);

};

#endif