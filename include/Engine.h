#ifndef ENGINE_H
#define ENGINE_H
#include "raylib.h"
#include "Particle.h"


#define WORLD_BOUND_X 1280
#define WORLD_BOUND_Y 800

#define GRAVITY -5.0f

#define FIXED_DTIME 0.05f

#define REPEL_FORCE 100

#define NUM_PARTICLES 500

#define MAX_VELOCITY 30

void GenerateParticles();
void GenerateChunks(int width, int height);

void ProcessParticles();

void sayHello();

#endif