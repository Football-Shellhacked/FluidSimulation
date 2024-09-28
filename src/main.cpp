#include "raylib.h"
#include "Particle.h"
#include <Engine.h>
#include <stdlib.h>
#include <stdio.h>
#include "resource_dir.h"

int amtWChunks = 10;
int amtHChunks = 5;

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
	GenerateChunks(amtWChunks,amtHChunks);
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
			else if (IsKeyPressed(KEY_TAB))
			{
				int value = atoi(name);
				letterCount = 0;
				NUM_PARTICLES = value;
				printf("%d\n",NUM_PARTICLES);
				name[letterCount] = '\0';
				DeleteStuff();
				GenerateChunks(amtWChunks, amtHChunks);
				GenerateParticles();

			}
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        if (mouseOnText) framesCounter++;
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