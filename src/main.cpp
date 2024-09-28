#include "Pause.h" 
#include "raylib.h"
#include "Particle.h"
#include <Engine.h>

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

extern Particle particles[NUM_PARTICLES];



int main ()
{
	sayHello();
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(WORLD_BOUND_X, WORLD_BOUND_Y, "Fluid Simulation");

	SearchAndSetResourceDir("resources");

	GenerateChunks(1,1);
	GenerateParticles();
	
	while (!WindowShouldClose())		
	{ 

		if (IsKeyPressed(KEY_SPACE)) {
    	Paused = !Paused; 

		if (Paused) 
			DrawText("Paused", 600, 380, 40, GREEN);

			}

		BeginDrawing();

		ClearBackground(BLACK);

		ProcessParticles();

		EndDrawing();
	}
	

	
	CloseWindow();
	return 0;
}