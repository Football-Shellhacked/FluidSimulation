#include "raylib.h"
#include "Particle.h"
#include <Engine.h>
#include <stdlib.h>
#include <stdio.h>
#include "resource_dir.h"

extern Particle particles[NUM_PARTICLES];

int main ()
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
	GenerateChunks(6,3);
	GenerateParticles();
	
	while (!WindowShouldClose())		
	{



		if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
        else mouseOnText = false;
        if (mouseOnText)
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            int key = GetCharPressed();
            while (key > 0)
            {
                
                if ((key >= 48) && (key <= 57) && (letterCount < 4))
                {
                    name[letterCount] = (int)key;
                    name[letterCount+1] = '\0';
                    letterCount++;
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                name[letterCount] = '\0';
            }
			else if (IsKeyPressed(KEY_SPACE))
			{
				int value = atoi(name);
				letterCount = 0;
				globalparticlecount = value;
				printf("%d\n",globalparticlecount);
				 name[letterCount] = '\0';

			}
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        if (mouseOnText) framesCounter++;
        else framesCounter = 0;




		BeginDrawing();
		ClearBackground(BLACK);
		ProcessParticles();



		DrawText("PLACE MOUSE OVER BOX TO CHANGE PARTICLE# AND PRESS SPACE TO CONFIRM", 130, 10, 20, GRAY);
            DrawRectangleRec(textBox, LIGHTGRAY);
            if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
            else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
            DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, RED);


		EndDrawing();
	}

	CloseWindow();
	return 0;
}