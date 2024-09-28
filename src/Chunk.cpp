#include "Chunk.h"
#include "ChunkSize.h"

Chunk::Chunk(){
    position = {0,0};
    
}
Vector2 Chunk::lowerLeftBound(){return {position.x-chunkSize.x/2, position.y + chunkSize.y/2};}
Vector2 Chunk::upperLeft(){return {position.x-chunkSize.x/2, position.y - chunkSize.y/2};}
Vector2 Chunk::lowerRightBound(){return {position.x+chunkSize.x/2, position.y + chunkSize.y/2};}
Vector2 Chunk::upperRightBound(){return {position.x+chunkSize.x/2, position.y - chunkSize.y/2};}