#include "Particle.h"
#include "Engine.h"
#include <cmath>

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
        velocity.x/=-2;
    }
    if(position.y+radius >= WORLD_BOUND_Y || position.y <= radius){
        position.y = clamp(position.y, radius+1, WORLD_BOUND_Y-radius-1);
        velocity.y/=-2;
    }
}

float magnitude(Vector2 vec){
    return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

Vector2 normalize(Vector2 vec){
    float mag = magnitude(vec);
    return {vec.x/mag, vec.y/mag};
}

void Particle::Repel(Particle* p){
    Vector2 diff = {p->position.x - position.x, p->position.y-position.y};
    float dist = magnitude(diff);
    if(dist < 0.0001f){return;}
    Vector2 dir = normalize(diff);
    float force = (REPEL_FORCE*FIXED_DTIME)/(dist*dist);
    velocity.x -= (dir.x) * force;
    velocity.y -= (dir.y) * force;
    if(magnitude(velocity) >= MAX_VELOCITY){
        velocity = normalize(velocity);
        velocity.x*=MAX_VELOCITY;
        velocity.y*=MAX_VELOCITY;
    }
}