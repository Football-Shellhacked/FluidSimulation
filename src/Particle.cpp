#include "Particle.h"
#include "Engine.h"
#include <cmath>
#include <algorithm>

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

void Particle::ResolveBoundColisions(){
    if (position.x + radius >= WORLD_BOUND_X) {
        position.x = WORLD_BOUND_X - radius - 1;
        velocity.x *= -0.5f;  // Invert and reduce speed
    } else if (position.x <= radius) {
        position.x = radius + 1;
        velocity.x *= -0.5f;
    }

    if (position.y + radius >= WORLD_BOUND_Y) {
        position.y = WORLD_BOUND_Y - radius - 1;
        velocity.y *= -0.5f;
    } else if (position.y <= radius) {
        position.y = radius + 1;
        velocity.y *= -0.5f;
    }
}

void Particle::Update() {
    // Get frame time once
    float frameTime = GetFrameTime();

    // Update position based on velocity
    position.x += velocity.x * FIXED_DTIME * frameTime;
    position.y += velocity.y * FIXED_DTIME * frameTime;

    // Apply gravity
    velocity.y -= GRAVITY * FIXED_DTIME * frameTime;

    // Limit the velocity to prevent particles from gaining excessive speed
    if (magnitude(velocity) > MAX_VELOCITY) {
        velocity = normalize(velocity);
        velocity.x *= MAX_VELOCITY;
        velocity.y *= MAX_VELOCITY;
    }

    // Resolve boundary collisions
    ResolveBoundColisions();
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
    float force = (REPEL_FORCE*FIXED_DTIME*GetFrameTime())/(dist*dist);
    velocity.x -= (dir.x) * force;
    velocity.y -= (dir.y) * force;
}

void Particle::AddForce(Vector2 force){
    velocity.x += force.x * GetFrameTime()*FIXED_DTIME;
    velocity.y += force.y * GetFrameTime()*FIXED_DTIME;
}