#include "FunFeature.h"

extern float clamp(float val, float min, float max);


bool FunFeatures::cursorInteraction=true;
float FunFeatures::DrawSlider(Rectangle slider, float minValue, float maxValue, float value, Color color) {
    // Calculate the position of the slider handle based on the current value
    float handlePosition = slider.x + ((value - minValue) / (maxValue - minValue)) * slider.width;

    // Draw the slider track
    DrawRectangleRec(slider, LIGHTGRAY);

    // Draw the slider handle
    DrawCircleV((Vector2){ handlePosition, slider.y + slider.height / 2 }, slider.height, color);

    // Check for mouse interaction with the slider
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, slider)) {
            // Update the slider value based on the mouse position
            value = ((mousePos.x - slider.x) / slider.width) * (maxValue - minValue) + minValue;
            value = clamp(value, minValue, maxValue); // Ensure value stays within min/max bounds
        }
    }

    return value;
}