#ifndef ENGINE_H
#define ENGINE_H
#include "raylib.h"
#include "Particle.h"


#define WORLD_BOUND_X 1280
#define WORLD_BOUND_Y 800

#define GRAVITY -4.5f

#define FIXED_DTIME 10.0f

#define REPEL_FORCE 10000

extern int NUM_PARTICLES;

#define MAX_VELOCITY 100

extern bool PAUSED;

void GenerateParticles();
void GenerateChunks(int width, int height);

void DeleteStuff();

void ProcessParticles();

void sayHello();

#endif