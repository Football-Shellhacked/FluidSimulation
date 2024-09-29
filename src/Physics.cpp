#include "Physics.h"
#include "Engine.h"
#include <algorithm>
#include <cmath>
#include "Chunk.h"
#include <stdio.h>

extern Chunk** chunks;

float smooth_rad_mult = 5.0f;

float Physics::TARGET_DENSITY=2.5f;
float Physics::PRESSURE_MULTIPLIER=0.5f;
float* Physics::DENSITIES = new float[NUM_PARTICLES];
float* Physics::PROPERTIES = new float[NUM_PARTICLES];

float Physics::SmoothingKernel(float dst, float radius) {
    if(dst >= radius) return 0;
    float volume = (PI * powf(radius, 4.0f))/6;
    return (radius -dst) * (radius - dst) / volume;
}


float Physics::SmoothingKernelDerivative(float dst, float radius){
    if(dst >= radius) return 0;
    float scale = 12 / (powf(radius,4)*PI);
    return (dst-radius)*scale;
}

float Physics::CalculateDensity(Particle* samplePoint){
    float density = 0.0f;
    const float mass = 0.1f;
    float smoothingRadius = samplePoint->radius * smooth_rad_mult;
    Chunk* chunk = (Chunk*)samplePoint->chunkPtr;
    int startChunkX = std::max(0, chunk->indX-1);
    int endChunkX = std::min(numWchunks-1, chunk->indX+1);
    int startChunkY = std::max(0, chunk->indY-1);
    int endChunkY = std::min(numHchunks-1, chunk->indY+1);
    //LOOP OVER PARTICLES IN NEIGHBORING CHUNKS
    for(int i = startChunkX; i <= endChunkX; i++){
        for(int j = startChunkY; j <= endChunkY; j++){
            //printf("LOOPING OVER CHUNK %d %d\n", i,j);
            for(void* ptr : chunks[i][j].particles){

                Particle* particle = (Particle*)ptr;

                if(particle->particleIndex == samplePoint->particleIndex)
                    continue;

                Vector2 diff = {particle->position.x - samplePoint->position.x, particle->position.y - samplePoint->position.y};
                float dst = magnitude(diff);
                if(dst <= 0.001f){continue;}
                float influence = SmoothingKernel(dst, smoothingRadius);
                density += mass*influence;
            }
        }
    }
    DENSITIES[samplePoint->particleIndex] = density;
    return density;
    
}

float Physics::CalculateProperty(Particle* samplePoint){
    float property = 0;
    float smoothingRadius = samplePoint->radius * smooth_rad_mult;
    const float mass = 1;
    Chunk* chunk = (Chunk*)samplePoint->chunkPtr;
    int startChunkX = std::max(0, chunk->indX-1);
    int endChunkX = std::min(numWchunks-1, chunk->indX+1);
    int startChunkY = std::max(0, chunk->indY-1);
    int endChunkY = std::min(numHchunks-1, chunk->indY+1);
    //LOOP OVER PARTICLES IN NEIGHBORING CHUNKS
    for(int i = startChunkX; i <= endChunkX; i++){
        for(int j = startChunkY; j <= endChunkY; j++){
            for(void* ptr : chunks[i][j].particles){

                Particle* particle = (Particle*)ptr;

                if(particle->particleIndex == samplePoint->particleIndex)
                    continue;

                Vector2 diff = {particle->position.x - samplePoint->position.x, particle->position.y - samplePoint->position.y};
                float dst = magnitude(diff);
                float influence = SmoothingKernel(dst, smoothingRadius);
                float density = DENSITIES[samplePoint->particleIndex];
                if(density != 0)
                    property += 1 * mass / density * influence;
            }
        }
    }
    return property;
}

Vector2 Physics::CalculatePropertyGradient(Particle* samplePoint){
    Vector2 propertyGradient;
    float smoothingRadius = samplePoint->radius * 2;
    const float mass = 1;
    Chunk* chunk = (Chunk*)samplePoint->chunkPtr;
    int startChunkX = std::max(0, chunk->indX-1);
    int endChunkX = std::min(numWchunks-1, chunk->indX+1);
    int startChunkY = std::max(0, chunk->indY-1);
    int endChunkY = std::min(numHchunks-1, chunk->indY+1);
    //LOOP OVER PARTICLES IN NEIGHBORING CHUNKS
    for(int i = startChunkX; i <= endChunkX; i++){
        for(int j = startChunkY; j <= endChunkY; j++){
            for(void* ptr : chunks[i][j].particles){

                Particle* particle = (Particle*)ptr;

                if(particle->particleIndex == samplePoint->particleIndex)
                    continue;

                Vector2 diff = {particle->position.x - samplePoint->position.x, particle->position.y - samplePoint->position.y};
                Vector2 dir = normalize(diff);
                float dst = magnitude(diff);
                if(dst <= 0.001f){continue;}
                float slope = SmoothingKernelDerivative(dst, smoothingRadius);
                float density = DENSITIES[samplePoint->particleIndex];
                float val = -PROPERTIES[samplePoint->particleIndex] * slope * mass / density;
                propertyGradient.x += dir.x * val;
                propertyGradient.y += dir.y * val;
            }
        }
    }
    return propertyGradient;
}

float Physics::ConvertDensityToPressure(float density){
    float densityError = density - TARGET_DENSITY;
    float pressure = densityError * PRESSURE_MULTIPLIER;
    return pressure;
}

float CalculateSharedPressure(float densityA, float densityB){
    float pressureA = Physics::ConvertDensityToPressure(densityA);
    float pressureB = Physics::ConvertDensityToPressure(densityB);
    return (pressureA + pressureB)/2;
}

Vector2 Physics::CalculatePressureForce(Particle* samplePoint) {
    Vector2 pressureForce = {0, 0};
    float smoothingRadius = samplePoint->radius * smooth_rad_mult;
    const float mass = 1;
    Chunk* chunk = (Chunk*)samplePoint->chunkPtr;
    int startChunkX = std::max(0, chunk->indX - 1);
    int endChunkX = std::min(numWchunks - 1, chunk->indX + 1);
    int startChunkY = std::max(0, chunk->indY - 1);
    int endChunkY = std::min(numHchunks - 1, chunk->indY + 1);

    // LOOP OVER PARTICLES IN NEIGHBORING CHUNKS
    for (int i = startChunkX; i <= endChunkX; i++) {
        for (int j = startChunkY; j <= endChunkY; j++) {
            for (void* ptr : chunks[i][j].particles) {
                Particle* particle = (Particle*)ptr;

                if (particle->particleIndex == samplePoint->particleIndex)
                    continue;

                Vector2 diff = {particle->position.x - samplePoint->position.x, particle->position.y - samplePoint->position.y};
                float dst = magnitude(diff);

                // Prevent division by zero or very small distance issues
                if (dst <= 0.001f) {
                    continue;
                }

                Vector2 dir = normalize(diff);
                float slope = SmoothingKernelDerivative(dst, smoothingRadius);

                // Check for invalid slope values
                if (std::isnan(slope) || std::isinf(slope)) {
                    continue;
                }

                float density = DENSITIES[samplePoint->particleIndex];
                if (density <= 0.0f) {
                    continue;
                }

                float sharedPressure = CalculateSharedPressure(density, DENSITIES[particle->particleIndex]);
                float val = sharedPressure * slope * mass / density;

                // Check for invalid force values
                if (std::isnan(val) || std::isinf(val)) {
                    continue;
                }
                pressureForce.x += dir.x * val / density;
                pressureForce.y += dir.y * val / density;
            }
        }
    }

    // Check final force values
    if (std::isnan(pressureForce.x) || std::isnan(pressureForce.y) ||
        std::isinf(pressureForce.x) || std::isinf(pressureForce.y)) {
        return {0, 0};  // Return zero force if invalid
    }

    return pressureForce;
}

float dotProduct(Vector2 vec1, Vector2 vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

void ResolveCircleCollision(Particle* p1, Particle* p2) {
    // Calculate the difference in positions
    Vector2 diff = {p2->position.x - p1->position.x, p2->position.y - p1->position.y};

    // Calculate the distance between particles
    float dist = magnitude(diff);

    // Calculate the sum of radii
    float radiusSum = p1->radius + p2->radius;

    // Check for collision
    if (dist < radiusSum) {
        // Calculate the collision normal (normalized difference vector)
        Vector2 collisionNormal;
        if (dist > 0.0001f) { // Avoid division by zero
            collisionNormal = {diff.x / dist, diff.y / dist};
        } else {
            // If particles are extremely close or overlapping exactly, set an arbitrary normal
            collisionNormal = {1.0f, 0.0f};
        }

        // Calculate the penetration depth
        float penetrationDepth = radiusSum - dist;

        // Separate the particles by moving them apart along the collision normal
        p1->position.x -= collisionNormal.x * penetrationDepth * 0.5f;
        p1->position.y -= collisionNormal.y * penetrationDepth * 0.5f;
        p2->position.x += collisionNormal.x * penetrationDepth * 0.5f;
        p2->position.y += collisionNormal.y * penetrationDepth * 0.5f;

        // Calculate the relative velocity of the particles
        Vector2 relativeVelocity = {p2->velocity.x - p1->velocity.x, p2->velocity.y - p1->velocity.y};

        // Calculate the velocity along the normal
        float velocityAlongNormal = dotProduct(relativeVelocity, collisionNormal);

        // Check if particles are moving towards each other
        if (velocityAlongNormal > 0) {
            return; // Do not resolve if particles are moving away from each other
        }

        // Calculate restitution (elasticity of collision, 1.0 = perfectly elastic, 0.0 = inelastic)
        float restitution = 0.8f; // You can adjust this value for different collision effects

        // Calculate impulse scalar
        float impulse = -(1 + restitution) * velocityAlongNormal;

        // Apply impulse directly to the velocities of each particle (ignoring mass)
        Vector2 imp = {impulse * collisionNormal.x, impulse*collisionNormal.y};
        Vector2 negImp = {-imp.x, -imp.y};
        p1->AddForce(negImp);
        p2->AddForce(imp);

        
    }
}


Vector2 Physics::CalculateRepelForce(Particle* samplePoint){
    Vector2 repelForce = {0, 0};
    float smoothingRadius = samplePoint->radius * smooth_rad_mult;
    Chunk* chunk = (Chunk*)samplePoint->chunkPtr;
    int startChunkX = std::max(0, chunk->indX - 1);
    int endChunkX = std::min(numWchunks - 1, chunk->indX + 1);
    int startChunkY = std::max(0, chunk->indY - 1);
    int endChunkY = std::min(numHchunks - 1, chunk->indY + 1);

    // LOOP OVER PARTICLES IN NEIGHBORING CHUNKS
    // for (int i = startChunkX; i <= endChunkX; i++) {
    //     for (int j = startChunkY; j <= endChunkY; j++) {
    //         for (void* ptr : chunks[i][j].particles) {
    //             Particle* particle = (Particle*)ptr;

    //             if (particle->particleIndex == samplePoint->particleIndex)
    //                 continue;

    //             Vector2 diff = {particle->position.x - samplePoint->position.x, particle->position.y - samplePoint->position.y};
    //             float dst = magnitude(diff);

    //             // Prevent division by zero or very small distance issues
    //             if (dst <= 0.001f) {
    //                 continue;
    //             }

    //             Vector2 dir = normalize(diff);
    //             float slope = SmoothingKernelDerivative(dst, smoothingRadius);
    //             repelForce.x += dir.x * slope * REPEL_FORCE;
    //             repelForce.y += dir.y * slope * REPEL_FORCE;
    //         }
    //     }
    // }
        const float MIN_DIST = 0.5f; // Minimum allowed distance between particles
        const float SEPARATION_STRENGTH = 10.0f; // Force applied for separation

        for (void* ptr : chunk->particles) {
            Particle *particle = (Particle *)ptr;

            if (particle->particleIndex == samplePoint->particleIndex)
                continue;
            
            ResolveCircleCollision(samplePoint, particle);

            Vector2 diff = {particle->position.x - samplePoint->position.x, particle->position.y - samplePoint->position.y};
            float dst = magnitude(diff);

            // Prevent division by zero or very small distance issues
            if (dst <= 0.001f)
            {
                continue;
            }

            // Apply minimum distance constraint with a separation force
            if (dst < MIN_DIST) {
                Vector2 separationForce = normalize(diff);
                separationForce.x *= SEPARATION_STRENGTH * (MIN_DIST - dst);
                separationForce.y *= SEPARATION_STRENGTH * (MIN_DIST - dst);
                repelForce.x += separationForce.x;
                repelForce.y += separationForce.y;
            } else {
                Vector2 dir = normalize(diff);
                float slope = SmoothingKernelDerivative(dst, smoothingRadius);
                repelForce.x += dir.x * slope * REPEL_FORCE;
                repelForce.y += dir.y * slope * REPEL_FORCE;
            }
        }
        return repelForce;

}