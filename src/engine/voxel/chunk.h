#ifndef CHUNK
#define CHUNK

#include "engine/common/rendergeometry/shape.h"

class Block;
class Blocks;

class Chunk : public Shape
{
public:
    Chunk(int chunkSize);
    ~Chunk();

    void setBlock(int x, int y, int z, int chunkSize, unsigned char type);

    void update();

    void draw();

    void genChunkData(int chunkSize, Blocks* b);

    void genEmpty();

    unsigned char getType(int x, int y, int z,  int chunkSize);
protected:
    unsigned char* m_data;

    inline glm::vec2 indexToTexel(int index, int texelDimensions);
};

#endif // CHUNK
