#include "raylib.h"
#include "Particle.h"
#include <Engine.h>
#include <stdlib.h>
#include <stdio.h>
#include "resource_dir.h"
#include "FunFeature.h"

int amtWChunks = 10;
int amtHChunks = 5;

extern float clamp(float val, float min, float max);

int main()
{
    int globalparticlecount = NUM_PARTICLES;

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(WORLD_BOUND_X, WORLD_BOUND_Y, "Liquid Simulation");

    char name[5] = "\0";
    int letterCount = 0;
    Rectangle textBox = { 10, 10, 110, 50 };
    bool mouseOnText = false;
    int framesCounter = 0;

    SearchAndSetResourceDir("resources");
    GenerateChunks(amtWChunks, amtHChunks);
    GenerateParticles();

    // Render Texture and Shader
    RenderTexture2D target = LoadRenderTexture(WORLD_BOUND_X, WORLD_BOUND_Y);
    Shader blurShader = LoadShader(0, "blur.fs");
    int blurStrengthLocation = GetShaderLocation(blurShader, "blurStrength");
    float blurStrength = 2.0f;

    // Check if shader is loaded correctly
    if (blurStrengthLocation == -1) {
        printf("Failed to get shader location for 'blurStrength'\n");
    }

    Rectangle cursorCheckbox = {WORLD_BOUND_X-100, 50, 50, 50};

    Rectangle useGravityBox = {WORLD_BOUND_X-100, 150, 50, 50}; 
    SetTargetFPS(60);  // Set the game to run at 60 frames per second

    while (!WindowShouldClose()) 
    {
        // Update input and controls
        if (CheckCollisionPointRec(GetMousePosition(), textBox)) {
            mouseOnText = true;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
        } else {
            mouseOnText = false;
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        if (mouseOnText) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 48) && (key <= 57) && (letterCount < 4)) {
                    name[letterCount] = (int)key;
                    name[letterCount + 1] = '\0';
                    letterCount++;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                name[letterCount] = '\0';
            } else if (IsKeyPressed(KEY_TAB)) {
                int value = atoi(name);
                letterCount = 0;
                NUM_PARTICLES = value;
                printf("NUM_PARTICLES: %d\n", NUM_PARTICLES);
                name[letterCount] = '\0';
                DeleteStuff();
                GenerateChunks(amtWChunks, amtHChunks);
                GenerateParticles();
            }
        }

        if (IsKeyPressed(KEY_SPACE)) {
            PAUSED = !PAUSED;
        }

        // Update the blur strength dynamically using keyboard input
        if (IsKeyDown(KEY_UP)) blurStrength += 1.0f;
        if (IsKeyDown(KEY_DOWN)) blurStrength -= 1.0f;

        blurStrength = clamp(blurStrength, 0.0f, 100.0f);  // Limit blur strength between 0 and 100
        SetShaderValue(blurShader, blurStrengthLocation, &blurStrength, SHADER_UNIFORM_FLOAT);

        // Render to texture
        BeginTextureMode(target);
        ClearBackground(BLACK);

        ProcessParticles();

        EndTextureMode();

        BeginDrawing();

        ClearBackground(BLACK);

        // Apply blur shader and draw the texture
        BeginShaderMode(blurShader);
        DrawTextureRec(target.texture, (Rectangle){0, 0, target.texture.width, -target.texture.height}, (Vector2){0, 0}, WHITE);
        EndShaderMode();

        // Draw UI elements on top of the blurred texture
        if (PAUSED) {
            DrawRectangle(1150, 60, 20, 60, WHITE);
            DrawRectangle(1180, 60, 20, 60, WHITE);
        }

        // Draw instructions and input box for changing particle count
        DrawText("PLACE MOUSE OVER BOX TO CHANGE PARTICLE# AND PRESS TAB TO CONFIRM", 130, 10, 20, GRAY);
        DrawRectangleRec(textBox, LIGHTGRAY);
        if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
        else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
        DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, RED);

        FunFeatures::cursorInteraction = FunFeatures::DrawCheckboxWithLabel(cursorCheckbox, FunFeatures::cursorInteraction, "Cursor Interaction", DARKGRAY); 

        FunFeatures::useGravity = FunFeatures::DrawCheckboxWithLabel(useGravityBox, FunFeatures::useGravity, "Gravity Enabled", DARKGRAY); 

        EndDrawing();
    }

    // Unload resources and close window
    UnloadShader(blurShader);
    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}
