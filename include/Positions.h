#ifndef POSITIONS_H
#define POSITIONS_H 

#include <vector>
#include "raylib.h"


extern std::vector<Vector2> particlePositions;

extern Particle* particles;

void DrawParticles(); 

#endif  