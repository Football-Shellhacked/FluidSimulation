#include "raylib.h"
#include "Particle.h"
#include <Engine.h>
#include <stdlib.h>
#include <stdio.h>
#include <FunFeature.h>
#include "resource_dir.h"

int amtWChunks = 10;
int amtHChunks = 5;

int main ()
{
	int globalparticlecount = NUM_PARTICLES;
	float globalgravitycount = GRAVITY;

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(WORLD_BOUND_X, WORLD_BOUND_Y, "Particle Simulation");



	char particledigit[5] = "\0";
    Rectangle particleBox = { 10, 10, 110, 50 };

	char gravitydigit[5] = "\0";
    Rectangle gravityBox = { 100, 100, 350, 15 };

	bool mouseOnParticle = false;
	bool mouseOnGravity = false;
    int framesCounter = 0;
	int letterCount = 0;


	SearchAndSetResourceDir("resources");
	GenerateChunks(amtWChunks,amtHChunks);
	GenerateParticles();
	
	while (!WindowShouldClose())		
	{



		if (CheckCollisionPointRec(GetMousePosition(), particleBox)) mouseOnParticle = true;
        else mouseOnParticle = false;
        if (mouseOnParticle)
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            int key = GetCharPressed();
            while (key > 0)
            {
                
                if ((key >= 48) && (key <= 57) && (letterCount < 4))
                {
                    particledigit[letterCount] = (int)key;
                    particledigit[letterCount+1] = '\0';
                    letterCount++;
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                particledigit[letterCount] = '\0';
            }
			else if (IsKeyPressed(KEY_TAB))
			{
				int value = atoi(particledigit);
				letterCount = 0;
				NUM_PARTICLES = value;
				printf("%d\n",NUM_PARTICLES);
				particledigit[letterCount] = '\0';
				DeleteStuff();
				GenerateChunks(amtWChunks, amtHChunks);
				GenerateParticles();

			}
        }

		
        
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        if (mouseOnParticle || mouseOnGravity) framesCounter++;
        else framesCounter = 0;

		
		BeginDrawing();
		ClearBackground(BLACK);
		if(IsKeyPressed(KEY_SPACE)){
			PAUSED = !PAUSED;
		}

		if (PAUSED) {
		DrawRectangle(1150, 60, 20, 60, WHITE);
		DrawRectangle(1180, 60, 20, 60, WHITE);
		} 

		ProcessParticles();


		DrawText("PLACE MOUSE OVER BOX TO CHANGE PARTICLE# AND PRESS TAB TO CONFIRM", 130, 30, 20, GRAY);
            DrawRectangleRec(particleBox, LIGHTGRAY);
            if (mouseOnParticle) DrawRectangleLines((int)particleBox.x, (int)particleBox.y, (int)particleBox.width, (int)particleBox.height, RED);
            else DrawRectangleLines((int)particleBox.x, (int)particleBox.y, (int)particleBox.width, (int)particleBox.height, DARKGRAY);
            DrawText(particledigit, (int)particleBox.x + 5, (int)particleBox.y + 8, 40, RED);

		GRAVITY = -FunFeatures::DrawSlider(gravityBox, -20.0f, 20.0f, -GRAVITY, WHITE);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
