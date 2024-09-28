#include "Engine.h"
#include <stdio.h>

Particle particles[NUM_PARTICLES];

void sayHello(){
    printf("Hello World!\n");
}

void ProcessParticles(){
    for(int i = 0; i < NUM_PARTICLES; i++){
        particles[i].Update();
    }
}