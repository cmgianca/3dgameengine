#ifndef VOXELMANAGER
#define VOXELMANAGER

#include "engine/common/manager.h"
#include "engine/voxel/chunk.h"
#include "util/CommonIncludes.h"
#include <qlist.h>

class Block;
class Blocks;
class Camera;
class Chunk;
class Graphics;
class World;

const int chunkSize = 32;

class VoxelManager : public Manager{
public:
    VoxelManager(int x, int y, int z, Blocks* b);
    virtual ~VoxelManager();

    struct QueueData{
        int x,y,z;
        Chunk* c;
    };

    void initialize();

    void draw(Graphics* g, Camera* c);

    void update(GLfloat seconds);

    Block* getBlock(int x, int y, int z);

    bool setBlock(int x, int y, int z, unsigned char type);

    Blocks* getBlocks();

    void setCenter(int x, int y, int z);

    RayCollision shootRay(const glm::vec3 &position, const glm::vec3 &direction, GLfloat length);

protected:
    Chunk** m_chunks;
    QList<QueueData> m_queue;
    Blocks* m_blocks;
    int m_width;
    int m_height;
    int m_depth;
    int centerX,centerY,centerZ;

    virtual void genChunk(int x, int y, int z, Chunk* c) = 0;

    Chunk* getChunk(int x, int y, int z);

    int getChunkSize();

    void continuousDetection(GLfloat seconds);

    void genEmpty(Chunk* c);

    bool isBlockTransparent(int x, int y, int z);

    int realModulo(int a, int b);

    int getCenterX() {return centerX;}
    int getCenterY() {return centerY;}
    int getCenterZ() {return centerZ;}
};


#endif // CHUNKMANAGER

