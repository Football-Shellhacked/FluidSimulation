#ifndef ENGINE_H
#define ENGINE_H
#include "raylib.h"
#include "Particle.h"


#define WORLD_BOUND_X 1000
#define WORLD_BOUND_Y 720

extern float GRAVITY;

#define FIXED_DTIME 8.0f

extern float REPEL_FORCE;

extern int NUM_PARTICLES;

#define MAX_VELOCITY 100

extern bool PAUSED;

void GenerateParticles();
void GenerateChunks(int width, int height);

void DeleteStuff();

void ProcessParticles();

void sayHello();

#endif