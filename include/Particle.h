#ifndef PARTICLE_H
#define PARTICLE_H
#include "raylib.h"

float magnitude(Vector2 vec);
Vector2 normalize(Vector2 vec);

class Particle{

    public:
        Vector2 position={0,0};
        Vector2 velocity = {0,0};

        float radius = 10;

        Color color = BLUE;

        void* chunkPtr=0;
        int particleIndex=0;
        int chunkParticleIndex=0;

        Particle();
        Particle(float x, float y, float vx, float vy, float radius);

        void Update();

        void Repel();

        void ResolveBoundColisions();

        void AddForce(Vector2 force);

        Color InterpolateColor();

};

#endif