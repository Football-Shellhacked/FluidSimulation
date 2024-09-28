#ifndef CHUNK_H
#define CHUNK_H
#include "raylib.h"
#include <vector>

extern int numHchunks;
extern int numWchunks;
extern int numChunks;

class Chunk{
    public:
        Vector2 position={0,0};
        int indX, indY;
        std::vector<void*> particles;
        Chunk();
        Vector2 lowerLeftBound();
        Vector2 upperLeft();
        Vector2 lowerRightBound();
        Vector2 upperRightBound();
};

#endif