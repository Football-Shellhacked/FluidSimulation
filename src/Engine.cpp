#include "Engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Chunk.h"
#include "ChunkSize.h"
#include "Physics.h"
#include "FunFeature.h"

int NUM_PARTICLES = 1000;
float GRAVITY = -9.8;
float REPEL_FORCE = 500;

Particle* particles;
Chunk** chunks;

bool PAUSED = false;

void sayHello(){
    printf("Hello World!\n");
}

void GenerateChunks(int width, int height){
    numHchunks = height;
    numWchunks = width;
    numChunks = width*height;
    chunks = new Chunk*[width];
    int cnt=0;
    chunkSize = {(float)WORLD_BOUND_X/width, (float)WORLD_BOUND_Y/height};
    for(int i = 1; i <= width; i++){
        chunks[i-1] = new Chunk[height];
        for(int j = 1; j <= height; j++){
            chunks[i-1][j-1].position = {(chunkSize.x/2)*i, (chunkSize.y/2)*j};
            chunks[i-1][j-1].indX = i-1;
            chunks[i-1][j-1].indY = j-1;
            cnt++;
        }
    }
}
bool withinBounds(Chunk* chunk, Particle* particle){
    return !(particle->position.x < chunk->lowerLeftBound().x || particle->position.x > chunk->lowerRightBound().x ||
     particle->position.y > chunk->lowerRightBound().y || particle->position.y < chunk->upperRightBound().y);
}
void AssignParticleToChunks(Particle* p){
    Chunk* currentChunk = (Chunk*)p->chunkPtr;

    if(currentChunk != NULL){
        if(withinBounds(currentChunk, p)){
            return;
        }
        std::swap(currentChunk->particles[p->chunkParticleIndex], currentChunk->particles.back());
        ((Particle*)currentChunk->particles[p->chunkParticleIndex])->chunkParticleIndex = p->chunkParticleIndex;
        currentChunk->particles.pop_back();
    }
    int chunkPosX = (p->position.x/WORLD_BOUND_X)*numWchunks;
    int chunkPosY = (p->position.y/WORLD_BOUND_Y)*numHchunks;
    Chunk* newChunk = &chunks[chunkPosX][chunkPosY];
    newChunk->particles.push_back(p);
    p->chunkParticleIndex = newChunk->particles.size()-1;
    p->chunkPtr = newChunk;
}

void GenerateParticles(){
    particles = new Particle[NUM_PARTICLES];
    srand(time(0));
    for(int i = 0; i < NUM_PARTICLES; i++){
        particles[i] = Particle(rand()%(WORLD_BOUND_X-300) + 150, rand()%(WORLD_BOUND_Y-150) + 75,0,0,5);
        particles[i].particleIndex = i;
        AssignParticleToChunks(&particles[i]);
    }
}


void ProcessParticles() {
    for (int i = 0; i < numWchunks; i++) {
        for (int j = 0; j < numHchunks; j++) {
            Chunk &c = chunks[i][j];

            for (int k = 0; k < c.particles.size(); k++) {
                Particle* a = (Particle*)c.particles[k];
                if (a != NULL) {
                    if(!PAUSED){
                        a->Update();
                        AssignParticleToChunks(a);

                        //Physics::CalculateDensity(a);

                        //a->AddForce(Physics::CalculatePressureForce(a));
                        if(FunFeatures::cursorInteraction){
                            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                                Vector2 mousePos = {(float)GetMouseX(), (float)GetMouseY()};
                                Vector2 diff = {mousePos.x - a->position.x, mousePos.y - a->position.y};
                                float dist = magnitude(diff);
                                Vector2 dir = normalize(diff);
                                float maxForce = 100.0f;
                                float falloff = maxForce / (dist * dist) * 10000;  // You can adjust this formula

                                // Optional: Clamp the falloff to avoid extreme forces
                                falloff = std::max(-maxForce, std::min(falloff, maxForce));

                                // Apply the falloff to the direction vector
                                dir.x *= falloff;
                                dir.y *= falloff;
                                a->AddForce(dir);
                            }
                            if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
                                Vector2 mousePos = {(float)GetMouseX(), (float)GetMouseY()};
                                Vector2 diff = {mousePos.x - a->position.x, mousePos.y - a->position.y};
                                Vector2 dir = normalize(diff);
                                float dist = magnitude(diff);
                                float maxForce = 100.0f;
                                float falloff = maxForce / (dist * dist) * -5000;  // You can adjust this formula

                                // Optional: Clamp the falloff to avoid extreme forces
                                falloff = std::max(-maxForce, std::min(falloff, maxForce));

                                // Apply the falloff to the direction vector
                                dir.x *= falloff;
                                dir.y *= falloff;
                                a->AddForce(dir);
                            }
                        }

                        a->Repel();
                    }
                    DrawCircle(a->position.x, a->position.y, a->radius, a->color);
                }
            }
        }
    }
}

void DeleteStuff(){
    delete [] particles;
    for(int i = 0; i < numWchunks; i++){
        delete [] chunks[i];
    }
    delete [] chunks;
}
