#ifndef ENGINE_H
#define ENGINE_H
#include "raylib.h"
#include "Particle.h"


#define WORLD_BOUND_X 1280
#define WORLD_BOUND_Y 800

#define GRAVITY -9.8f

#define FIXED_DTIME 5.0f

#define REPEL_FORCE 25

#define NUM_PARTICLES 1000

#define MAX_VELOCITY 100

void GenerateParticles();
void GenerateChunks(int width, int height);

void ProcessParticles();

void sayHello();

#endif