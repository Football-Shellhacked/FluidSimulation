#include "raylib.h"
#include "Particle.h"
#include <Engine.h>
#include <stdlib.h>
#include <stdio.h>
#include "resource_dir.h"
#include "FunFeature.h"

extern Particle* particles;
int amtWChunks = 10;
int amtHChunks = 5;
Color DARK_DARKGRAY = Color{25,25,25,255};
Color availableColors[] = {RED, GREEN, BLUE, YELLOW, ORANGE, PURPLE, MAROON, LIME, VIOLET, BROWN, RAYWHITE, BLACK, DARK_DARKGRAY};
const char* colorNames[] = {"RED", "GREEN", "BLUE", "YELLOW", "ORANGE", "PURPLE", "MAROON", "LIME", "VIOLET", "BROWN", "RAYWHITE", "BLACK", "DARK GRAY"};
int selectedColorIndex = 2;
int fastColorIndex = 10;


extern Color slowColor;
extern Color fastColor;

extern float clamp(float val, float min, float max);

int main()
{
    
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(WORLD_BOUND_X+200, WORLD_BOUND_Y+150, "Particle Simulation");

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
    float blurStrength = 10.0f;

    Rectangle cursorCheckbox = {WORLD_BOUND_X+115, 15, 25, 25};

    Rectangle useGravityBox = {WORLD_BOUND_X+115, 80, 25, 25}; 

	bool dropdownActive2 = false; 
	Rectangle dropdownBox2 = {1000, 450, 190, 20};

	bool dropdownActive = false; 
	Rectangle dropdownBox = {1000, 125, 190, 20}; 
	//Rectangle dropdownBox = {WORLD_BOUND_X / 2- 10, 400, 200, 30}; 
	int dropdownSelection = -1;



    // Check if shaders are loaded correctly
    if (blurStrengthLocationH == -1 || blurStrengthLocationV == -1) {
        printf("Failed to get shader location for 'blurStrength'\n");
    }

    SetTargetFPS(60);  // Set the game to run at 60 frames per second

    while (!WindowShouldClose()) 
    {

		Vector2 mousePosition = GetMousePosition();

		//Dropdown cursor detection
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            printf("Mouse clicked at: x = %f, y = %f\n", mousePosition.x, mousePosition.y);

            // Toggle dropdown menu when clicking on the dropdown box
            if (CheckCollisionPointRec(mousePosition, dropdownBox)) {
                printf("Clicked on dropdown box\n");
                dropdownActive = !dropdownActive;  // Open or close the dropdown

            }
			if (CheckCollisionPointRec(mousePosition, dropdownBox2)) {
                printf("Clicked on dropdown box\n");
                dropdownActive2 = !dropdownActive2;
				
			}
        }
		
		

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


		for (int i = 0; i < NUM_PARTICLES; i++) {
			particles[i].color = availableColors[selectedColorIndex];
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
            DrawRectangle(WORLD_BOUND_X/2-15, WORLD_BOUND_Y/2 - 30, 20, 60, LIGHTGRAY);
            DrawRectangle(WORLD_BOUND_X/2+15, WORLD_BOUND_Y/2 - 30, 20, 60, LIGHTGRAY);
        }	


		DrawRectangleRec(dropdownBox2, LIGHTGRAY);
		DrawText(colorNames[fastColorIndex], dropdownBox2.x + 10, dropdownBox2.y+5, 15, availableColors[fastColorIndex]); 
		DrawRectangleLines(dropdownBox2.x, dropdownBox2.y, dropdownBox2.width, dropdownBox2.height, RAYWHITE);


		DrawRectangleRec(dropdownBox, LIGHTGRAY);
		DrawText(colorNames[selectedColorIndex], dropdownBox.x + 10, dropdownBox.y+5, 15, availableColors[selectedColorIndex]); 
		DrawRectangleLines(dropdownBox.x, dropdownBox.y, dropdownBox.width, dropdownBox.height, RAYWHITE);

		if (dropdownActive)
		{

			for (int i = 0; i < sizeof(colorNames) / sizeof(colorNames[0]); i++)
			{
				Rectangle optionBox = {dropdownBox.x, dropdownBox.y + (i + 1) * 20, dropdownBox.width, 20};
				DrawRectangleRec(optionBox, LIGHTGRAY);
				DrawText(colorNames[i], optionBox.x, optionBox.y, 15, availableColors[i]);
				DrawRectangleLines(optionBox.x, optionBox.y, optionBox.width, optionBox.height, RAYWHITE);

				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					if (CheckCollisionPointRec(mousePosition, optionBox))
					{
						printf("Clicked on option: %s\n", colorNames[i]);
						selectedColorIndex = i; // Update selected color
						dropdownActive = false; // Close the dropdown
						slowColor = availableColors[i];
						break;
					}
				}
			}
		}

		if (dropdownActive2)
		{

			for (int i = 0; i < sizeof(colorNames) / sizeof(colorNames[0]); i++)
			{
				Rectangle optionBox = {dropdownBox2.x, dropdownBox2.y + (i + 1) * 20, dropdownBox2.width, 20};
				DrawRectangleRec(optionBox, LIGHTGRAY);
				DrawText(colorNames[i], optionBox.x, optionBox.y + 5, 15, availableColors[i]);
				DrawRectangleLines(optionBox.x, optionBox.y, optionBox.width, optionBox.height, RAYWHITE);

				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					if (CheckCollisionPointRec(mousePosition, optionBox))
					{
						printf("Clicked on option: %s\n", colorNames[i]);
						fastColorIndex = i;		 // Update selected color
						dropdownActive2 = false; // Close the dropdown
						fastColor = availableColors[i];
						break;
					}
				}
			}
		}

		// Draw instructions and input box for changing particle count
        DrawText("HOVER OVER BOXZ TO INPUT PARTICLE#. TAB TO CONFIRM.", 130, 27, 20, GRAY);
        DrawRectangleRec(particleBox, LIGHTGRAY);
        if (mouseOnParticle) DrawRectangleLines((int)particleBox.x, (int)particleBox.y, (int)particleBox.width, (int)particleBox.height, RED);
        else DrawRectangleLines((int)particleBox.x, (int)particleBox.y, (int)particleBox.width, (int)particleBox.height, DARKGRAY);
        DrawText(particledigit, (int)particleBox.x + 5, (int)particleBox.y + 8, 40, RED);
		GRAVITY = -FunFeatures::DrawSlider(gravityBox, -20.0f, 20.0f, -GRAVITY, WHITE);
		DrawText("GRAVITY SLIDER", 370, 98, 20, GRAY); 

        FunFeatures::cursorInteraction = FunFeatures::DrawCheckboxWithLabel(cursorCheckbox, FunFeatures::cursorInteraction, "Cursor Interaction", DARKGRAY); 

        FunFeatures::useGravity = FunFeatures::DrawCheckboxWithLabel(useGravityBox, FunFeatures::useGravity, "Gravity Enabled", DARKGRAY);

        DrawRectangle(0, WORLD_BOUND_Y, WORLD_BOUND_X, 150, DARKGRAY);
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

