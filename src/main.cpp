#include "Pause.h" 
#include "raylib.h"
#include "Particle.h"
#include <Engine.h>
#include "Positions.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

void DrawParticles();

bool Paused = false;

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

		if (IsKeyPressed(KEY_SPACE)) 
    		Paused = !Paused; 


		BeginDrawing();

		ClearBackground(BLACK);

		
		ProcessParticles();




		EndDrawing();

		if (Paused) {
			DrawParticles();
		} 
		else {
			ProcessParticles();
		}



		if (Paused) {
			DrawRectangle( 1150, 60, 20, 60, WHITE);
			DrawRectangle( 1180, 60, 20, 60, WHITE);
					}
		
	}
	

	
	CloseWindow();
	return 0;
}