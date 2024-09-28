

#include "raylib.h"
#include "Particle.h"
#include <Engine.h>

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

extern Particle particles[NUM_PARTICLES];

int main ()
{
	sayHello();
	// Tell the window to use vysnc and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(WORLD_BOUND_X, WORLD_BOUND_Y, "Fluid Simulation");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	GenerateChunks(1,1);
	GenerateParticles();
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the backbuffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		ProcessParticles();
		// draw our texture to the screen


		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}
	
	// cleanup
	// unload our texture so it can be cleaned u

	// destory the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}