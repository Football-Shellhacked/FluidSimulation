#include "Engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Chunk.h"
#include "ChunkSize.h"

Particle* particles = new Particle[NUM_PARTICLES];

Chunk** chunks;


void sayHello(){
    printf("Hello World!\n");
}
int numHchunks;
int numWchunks;
int numChunks;
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
    srand(time(0));
    for(int i = 0; i < NUM_PARTICLES; i++){
        particles[i] = Particle(rand()%WORLD_BOUND_X, rand()%WORLD_BOUND_Y,0,0,5);
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
                    a->Update();
                    AssignParticleToChunks(a);
                    DrawCircle(a->position.x, a->position.y, a->radius, BLUE);

                    for (int ni = std::max(0, i - 1); ni <= std::min(numWchunks - 1, i + 1); ni++) {
                        for (int nj = std::max(0, j - 1); nj <= std::min(numHchunks - 1, j + 1); nj++) {
                            Chunk &neighborChunk = chunks[ni][nj];

                            for (int l = 0; l < neighborChunk.particles.size(); l++) {
                                Particle* b = (Particle*)neighborChunk.particles[l];
                                if (b != NULL) {
                                    if (a->particleIndex == b->particleIndex) { continue; }
                                    a->Repel(b);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
