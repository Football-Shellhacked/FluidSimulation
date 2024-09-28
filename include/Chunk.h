#ifndef CHUNK_H
#define CHUNK_H
#include "raylib.h"
#include <vector>

class Chunk{
    public:
        Vector2 position={0,0};
        std::vector<void*> particles;
        Chunk();
        Vector2 lowerLeftBound();
        Vector2 upperLeft();
        Vector2 lowerRightBound();
        Vector2 upperRightBound();
};

#endif