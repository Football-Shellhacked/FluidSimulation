#include "Particle.h"
#include "Engine.h"

float clamp(float val, float min, float max){
    return val > max ? max : (val < min ? min : val);
}

Particle::Particle(){
    position = {0,0};
    velocity = {0,0};
    radius = 10;
}

Particle::Particle(float x, float y, float vx, float vy, float radius){
    position.x = x;
    position.y = y;
    velocity.x = vx;
    velocity.y = vy;
    this->radius = radius;
}

void Particle::Update(){
    position.x += velocity.x * FIXED_DTIME;
    position.y += velocity.y * FIXED_DTIME;
    velocity.y -= GRAVITY * FIXED_DTIME;
    ResolveBoundColisions();
}

void Particle::ResolveBoundColisions(){
    if(position.x+radius >= WORLD_BOUND_X || position.x <= radius){
        position.x = clamp(position.x, radius+1, WORLD_BOUND_X-radius-1);
        velocity.x/=-1.2;
    }
    if(position.y+radius >= WORLD_BOUND_Y || position.y <= radius){
        position.y = clamp(position.y, radius+1, WORLD_BOUND_Y-radius-1);
        velocity.y/=-1.2;
    }
}