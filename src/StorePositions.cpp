#include "Pause.h" 
#include "raylib.h"
#include "Particle.h"
#include <Engine.h>
#include "Positions.h"

extern Particle particles[NUM_PARTICLES];

void DrawParticles() {
for (int i = 0; i < NUM_PARTICLES; i++) {
    DrawCircle(particlePositions[i].x, particlePositions[i].y, particles[i].radius, BLUE);
     }
}
