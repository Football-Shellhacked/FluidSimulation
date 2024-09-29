#include "FunFeature.h"

bool FunFeatures::cursorInteraction=true;
bool FunFeatures::useGravity=true;

bool FunFeatures::DrawCheckboxWithLabel(Rectangle bounds, bool checked, const char* label, Color color) {
    DrawRectangleLinesEx(bounds, 2, color);

    if (checked) {
        DrawLine(bounds.x + 4, bounds.y + bounds.height / 2,
                 bounds.x + bounds.width / 2, bounds.y + bounds.height - 4, color);
        DrawLine(bounds.x + bounds.width / 2, bounds.y + bounds.height - 4,
                 bounds.x + bounds.width - 4, bounds.y + 4, color);
    }

    DrawText(label, bounds.x - bounds.width*2, bounds.y + bounds.height, 15, color);

    if (CheckCollisionPointRec(GetMousePosition(), bounds) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        checked = !checked;
    }

    return checked;
}