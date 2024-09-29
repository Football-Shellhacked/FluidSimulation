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
    
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(WORLD_BOUND_X, WORLD_BOUND_Y, "Particle Simulation");

    char particledigit[5] = "\0";
	Rectangle particleBox = { 10, 10, 110, 50 };
	Rectangle gravityBox = { 10, 100, 350, 15 };
    int letterCount = 0;
    bool mouseOnParticle = false;
    int framesCounter = 0;

    SearchAndSetResourceDir("resources");
    GenerateChunks(amtWChunks, amtHChunks);
    GenerateParticles();

    // Render Textures and Shaders
    RenderTexture2D sceneTexture = LoadRenderTexture(WORLD_BOUND_X, WORLD_BOUND_Y); // Scene render texture
    RenderTexture2D blurTexture = LoadRenderTexture(WORLD_BOUND_X, WORLD_BOUND_Y);  // Temporary blur texture

    Shader blurHShader = LoadShader(0, "blur_horizontal.fs"); // Horizontal blur shader
    Shader blurVShader = LoadShader(0, "blur_vertical.fs");   // Vertical blur shader

    int blurStrengthLocationH = GetShaderLocation(blurHShader, "blurStrength");
    int blurStrengthLocationV = GetShaderLocation(blurVShader, "blurStrength");
    float blurStrength = 3.0f;

    Rectangle cursorCheckbox = {WORLD_BOUND_X-100, 50, 50, 50};

    Rectangle useGravityBox = {WORLD_BOUND_X-100, 150, 50, 50}; 

    // Check if shaders are loaded correctly
    if (blurStrengthLocationH == -1 || blurStrengthLocationV == -1) {
        printf("Failed to get shader location for 'blurStrength'\n");
    }

    SetTargetFPS(60);  // Set the game to run at 60 frames per second

    while (!WindowShouldClose()) 
    {
        // Update input and controls
        if (CheckCollisionPointRec(GetMousePosition(), particleBox)) {
            mouseOnParticle = true;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
        } else {
            mouseOnParticle = false;
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        if (mouseOnParticle) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 48) && (key <= 57) && (letterCount < 4)) {
                    particledigit[letterCount] = (int)key;
                    particledigit[letterCount + 1] = '\0';
                    letterCount++;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                particledigit[letterCount] = '\0';

            } else if (IsKeyPressed(KEY_TAB)) {
                int value = atoi(particledigit);
                letterCount = 0;
                NUM_PARTICLES = value;
                printf("NUM_PARTICLES: %d\n", NUM_PARTICLES);
                particledigit[letterCount] = '\0';
                DeleteStuff();
                GenerateChunks(amtWChunks, amtHChunks);
                GenerateParticles();
            }
        }

        if (IsKeyPressed(KEY_SPACE)) {
            PAUSED = !PAUSED;
        }

        // Update the blur strength dynamically using keyboard input
        if (IsKeyDown(KEY_UP)) blurStrength += 0.1f;
        if (IsKeyDown(KEY_DOWN)) blurStrength -= 0.1f;

        blurStrength = clamp(blurStrength, 0.1f, 5.0f);  // Limit blur strength between 0.1 and 5
        SetShaderValue(blurHShader, blurStrengthLocationH, &blurStrength, SHADER_UNIFORM_FLOAT);
        SetShaderValue(blurVShader, blurStrengthLocationV, &blurStrength, SHADER_UNIFORM_FLOAT);

        // Render the scene to the scene texture
        BeginTextureMode(sceneTexture);
        ClearBackground(BLACK);
        ProcessParticles(); // Render your particles or scene here
        EndTextureMode();

        // Apply horizontal blur to the scene texture
        BeginTextureMode(blurTexture);
        BeginShaderMode(blurHShader);
        DrawTextureRec(sceneTexture.texture, (Rectangle){0, 0, (float)sceneTexture.texture.width, -(float)sceneTexture.texture.height}, (Vector2){0, 0}, WHITE);
        EndShaderMode();
        EndTextureMode();

        // Apply vertical blur to the horizontally blurred texture
        BeginDrawing();
        ClearBackground(BLACK);
        BeginShaderMode(blurVShader);
        DrawTextureRec(blurTexture.texture, (Rectangle){0, 0, (float)blurTexture.texture.width, -(float)blurTexture.texture.height}, (Vector2){0, 0}, WHITE);
        EndShaderMode();

        // Draw UI elements on top of the blurred texture
        if (PAUSED) {
            DrawRectangle(1150, 60, 20, 60, WHITE);
            DrawRectangle(1180, 60, 20, 60, WHITE);
        }

        // Draw instructions and input box for changing particle count
        DrawText("PLACE MOUSE OVER BOX TO CHANGE PARTICLE# AND PRESS TAB TO CONFIRM", 130, 27, 20, GRAY);
        DrawRectangleRec(particleBox, LIGHTGRAY);
        if (mouseOnParticle) DrawRectangleLines((int)particleBox.x, (int)particleBox.y, (int)particleBox.width, (int)particleBox.height, RED);
        else DrawRectangleLines((int)particleBox.x, (int)particleBox.y, (int)particleBox.width, (int)particleBox.height, DARKGRAY);
        DrawText(particledigit, (int)particleBox.x + 5, (int)particleBox.y + 8, 40, RED);
		GRAVITY = -FunFeatures::DrawSlider(gravityBox, -20.0f, 20.0f, -GRAVITY, WHITE);
		DrawText("GRAVITY SLIDER", 370, 98, 20, GRAY);

        FunFeatures::cursorInteraction = FunFeatures::DrawCheckboxWithLabel(cursorCheckbox, FunFeatures::cursorInteraction, "Cursor Interaction", DARKGRAY); 

        FunFeatures::useGravity = FunFeatures::DrawCheckboxWithLabel(useGravityBox, FunFeatures::useGravity, "Gravity Enabled", DARKGRAY); 
        EndDrawing();
    }

    // Unload resources and close window
    UnloadShader(blurHShader);
    UnloadShader(blurVShader);
    UnloadRenderTexture(sceneTexture);
    UnloadRenderTexture(blurTexture);
    CloseWindow();
    return 0;
}