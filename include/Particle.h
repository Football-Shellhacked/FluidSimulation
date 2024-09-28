#ifndef PARTICLE_H
#define PARTICLE_H
#include "raylib.h"

class Particle{

    public:
        Vector2 position;
        Vector2 velocity;

        float radius;

        Particle();
        Particle(float x, float y, float vx, float vy, float radius);

        void Update();

        void ResolveBoundColisions();

};

#endif