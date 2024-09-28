#include "Particle.h"
#include "Engine.h"
#include "Physics.h"
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

Color slowColor = BLUE;
Color fastColor = WHITE;

void Particle::InterpolateColor(){
    float val = magnitude(velocity)/MAX_VELOCITY * 2;
    if (val < 0.0f) val = 0.0f;
    if (val > 1.0f) val = 1.0f;

    // Interpolate each component separately
    Color result;
    result.r = static_cast<unsigned char>(std::round(BLUE.r + (WHITE.r - BLUE.r) * val));
    result.g = static_cast<unsigned char>(std::round(BLUE.g + (WHITE.g - BLUE.g) * val));
    result.b = static_cast<unsigned char>(std::round(BLUE.b + (WHITE.b - BLUE.b) * val));
    result.a = static_cast<unsigned char>(std::round(BLUE.a + (WHITE.a - BLUE.a) * val));

    color = result;

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
    InterpolateColor();

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

void Particle::Repel(){
    Vector2 repelForce = Physics::CalculateRepelForce(this);
    velocity.x += repelForce.x;
    velocity.y += repelForce.y;
}

void Particle::AddForce(Vector2 force){
    velocity.x = force.x * GetFrameTime()*FIXED_DTIME;
    velocity.y = force.y * GetFrameTime()*FIXED_DTIME;
}