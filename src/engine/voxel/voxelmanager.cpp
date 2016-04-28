#include "engine/common/camera.h"
#include "engine/common/graphics.h"
#include "engine/common/world.h"
#include "engine/common/noise/valuenoise.h"
#include "engine/common/entity.h"
#include "engine/common/collisiongeometry/boundingbox.h"
#include "engine/voxel/blocks.h"
#include "engine/voxel/voxelmanager.h"
#include <ctime>

VoxelManager::VoxelManager(int x, int y, int z, Blocks* b) :
    m_blocks(b),
    m_width(x),
    m_height(y),
    m_depth(z),
    centerX(0),
    centerY(0),
    centerZ(0)
{
    m_chunks = new Chunk*[m_width*m_height*m_depth];
}

VoxelManager::~VoxelManager() {
    delete m_blocks;
    for (int x = 0; x < m_width; x++) {
        for (int y = 0; y < m_height; y++) {
            for (int z = 0; z < m_depth; z++) {
                delete m_chunks[(x)*m_height*m_depth+y*m_depth+(z)];
            }
        }
    }
    delete[] m_chunks;
}

int VoxelManager::realModulo(int a, int b) {
    return (a%b+b)%b;
}

void VoxelManager::update(GLfloat seconds) {
    Manager::update(seconds);
    if (!m_queue.empty()) {
        QueueData q = m_queue.takeFirst();
        genChunk(q.x,q.y,q.z,q.c);
        q.c->genChunkData(chunkSize,m_blocks);
    }
}

void VoxelManager::setCenter(int x, int y, int z) {
    if (x != centerX || z != centerZ) {
        int xDiff = x-centerX;
        int zDiff = z-centerZ;
        centerX = x;
        centerY = y;
        centerZ = z;
        Chunk *worldCopy[m_width*m_height*m_depth];
        for (int x = centerX-m_width/2; x <= centerX+m_width/2;x++) {
            for (int y = 0; y < m_height; y++) {
                for (int z = centerZ-m_depth/2; z <= centerZ+m_depth/2; z++) {
                    worldCopy[realModulo(x-xDiff-centerX+m_width/2,m_width)*m_height*m_depth+y*m_depth+realModulo(z-zDiff-centerZ+m_depth/2,m_depth)] =
                            m_chunks[(x-centerX+m_width/2)*m_height*m_depth+y*m_depth+(z-centerZ+m_depth/2)];
                }
            }
        }
        memcpy(m_chunks,worldCopy,sizeof(Chunk*)*m_width*m_height*m_depth);
        for (int x = centerX-m_width/2; x <= centerX+m_width/2;x++) {
            for (int y = 0; y < m_height; y++) {
                for (int z = centerZ-m_depth/2; z <= centerZ+m_depth/2; z++) {
                    if (x+xDiff < centerX-m_width/2 || x+xDiff > centerX+m_width/2 || z+zDiff < centerZ-m_depth/2 || z+zDiff > centerZ+m_depth/2) {
                        genEmpty(m_chunks[(x-centerX+m_width/2)*m_height*m_depth+y*m_depth+(z-centerZ+m_depth/2)]);
                        m_chunks[(x-centerX+m_width/2)*m_height*m_depth+y*m_depth+(z-centerZ+m_depth/2)]->genEmpty();
                        m_queue.push_back({x,y,z,m_chunks[(x-centerX+m_width/2)*m_height*m_depth+y*m_depth+(z-centerZ+m_depth/2)]});
                    }
                }
            }
        }
    }
}

Chunk* VoxelManager::getChunk(int x, int y, int z) {
    int chunkX = floor(1.f*x/chunkSize);
    int chunkY = floor(1.f*y/chunkSize);
    int chunkZ = floor(1.f*z/chunkSize);
    if (chunkX < centerX-m_width/2 || chunkX > centerX+m_width/2 || chunkY < 0 || chunkY >= m_height || chunkZ < centerZ-m_depth/2 || chunkZ > centerZ+m_depth/2) {
        return NULL;
    } else {
        return m_chunks[(chunkX-centerX+m_width/2)*m_height*m_depth+chunkY*m_depth+(chunkZ-centerZ+m_depth/2)];
    }
}

void VoxelManager::initialize() {
    for (int x = 0; x < m_width; x++) {
        for (int y = 0; y < m_height; y++) {
            for (int z = 0; z < m_depth; z++) {
                m_chunks[(x)*m_height*m_depth+y*m_depth+(z)] = new Chunk(chunkSize);
            }
        }
    }

    const clock_t begin_time = clock();

    for (int x = centerX-m_width/2; x <= centerX+m_width/2; x++) {
        for (int y = 0; y < m_height; y++) {
            for (int z = centerZ-m_depth/2; z <= centerZ+m_depth/2; z++) {
                genChunk(x,y,z,m_chunks[(x-centerX+m_width/2)*m_height*m_depth+y*m_depth+(z-centerZ+m_depth/2)]);
            }
        }
    }
    std::cout << "Terrain Generation time: " << float(clock ()-begin_time )/CLOCKS_PER_SEC << std::endl;
    const clock_t data_time = clock();

    for (int x = centerX-m_width/2; x <= centerX+m_width/2; x++) {
        for (int y = 0; y < m_height; y++) {
            for (int z = centerZ-m_depth/2; z <= centerZ+m_depth/2; z++) {
                m_chunks[(x-centerX+m_width/2)*m_height*m_depth+y*m_depth+(z-centerZ+m_depth/2)]->genChunkData(chunkSize, m_blocks);
            }
        }
    }
    std::cout << "Vertex Generation time: " << float(clock ()-data_time )/CLOCKS_PER_SEC << std::endl;
    std::cout << "Total Time: " << float(clock ()-begin_time )/CLOCKS_PER_SEC << std::endl;
}

void VoxelManager::draw(Graphics* g, Camera* c) {
    int positions[3*m_width*m_height*m_depth] = {};
    bool returnValue[m_width*m_height*m_depth] = {};
    int index = 0;
    //View Frustrum culling
    for (int x = centerX-m_width/2; x <= centerX+m_width/2; x++) {
        for (int y = 0; y < m_height; y++) {
            for (int z = centerZ-m_depth/2; z <= centerZ+m_depth/2; z++) {
                positions[index] = x*chunkSize;
                positions[index+1] = y*chunkSize;
                positions[index+2] = z*chunkSize;
                index+= 3;
            }
        }
    }
    c->checkAABB(chunkSize, m_width*m_height*m_depth, positions, returnValue);
    for (int x = centerX-m_width/2; x <= centerX+m_width/2; x++) {
        for (int y = 0; y < m_height; y++) {
            for (int z = centerZ-m_depth/2; z <= centerZ+m_depth/2; z++) {
                if (returnValue[(x-centerX+m_width/2)*m_height*m_depth+y*m_depth+(z-centerZ+m_depth/2)]) {
                    g->draw(m_chunks[(x-centerX+m_width/2)*m_height*m_depth+y*m_depth+(z-centerZ+m_depth/2)], "terrain", glm::translate(glm::mat4(), glm::vec3(x*chunkSize,y*chunkSize,z*chunkSize)));
                }
            }
        }
    }
    Manager::draw(g,c);
}

Blocks* VoxelManager::getBlocks() {
    return m_blocks;
}

int VoxelManager::getChunkSize() {
    return chunkSize;
}

Block* VoxelManager::getBlock(int x, int y, int z) {
    Chunk* c = getChunk(x,y,z);
    int blockX = (x%chunkSize+chunkSize)%chunkSize;
    int blockY = (y%chunkSize+chunkSize)%chunkSize;
    int blockZ = (z%chunkSize+chunkSize)%chunkSize;

    if (c == NULL) {
        return NULL;
    }
    return m_blocks->getBlock(c->getType(blockX,blockY,blockZ,chunkSize));
}

bool VoxelManager::setBlock(int x, int y, int z, unsigned char type) {
    Chunk* c = getChunk(x,y,z);
    int blockX = realModulo(x,chunkSize);
    int blockY = realModulo(y,chunkSize);
    int blockZ = realModulo(z,chunkSize);

    if (c == NULL) {
        return false;
    }
    c->setBlock(blockX, blockY, blockZ,chunkSize, type);
    c->genChunkData(chunkSize,m_blocks);
    return true;
}

void VoxelManager::genEmpty(Chunk* c) {
    for (int x = 0; x < chunkSize; x++) {
        for (int y = 0; y < chunkSize; y++) {
            for (int z = 0; z < chunkSize; z++) {
                c->setBlock(x,y,z,chunkSize,0);
            }
        }
    }
}

void VoxelManager::continuousDetection(GLfloat seconds) {
    QList<Entity*>::iterator i;
    for (i = entities_.begin(); i != entities_.end(); ++i) {
        BoundingBox* box = (BoundingBox*) (*i)->getCollisionShape();
        glm::vec3 pos = box->getPosition();
        glm::vec3 distance = (*i)->getVelocity()*seconds;

        for (int y = floor(pos.y-box->getHeight()/2)-1; y+1 > pos.y-box->getHeight()/2 + distance.y; y--) {
            for (int x = floor(pos.x-box->getWidth()/2); x <= floor(pos.x+box->getWidth()/2);x++) {
                for (int z = floor(pos.z-box->getDepth()/2); z <= floor(pos.z+box->getDepth()/2);z++) {
                    Block* b = getBlock(x,y,z);
                    if (b && !b->passable) {
                        (*i)->setVelocity((*i)->getVelocity()*glm::vec3(1,0,1));
                        (*i)->setPosition((*i)->getPosition()*glm::vec3(1,0,1)+glm::vec3(0,y+1+box->getHeight()/2+.001f,0));
                    }
                }
            }
        }
        for (int y = floor(pos.y+box->getHeight()/2)+1; y < (pos.y+box->getHeight()/2 + distance.y); y++) {
            for (int x = floor(pos.x-box->getWidth()/2); x < ceil(pos.x+box->getWidth()/2);x++) {
                for (int z = floor(pos.z-box->getDepth()/2); z < ceil(pos.z+box->getDepth()/2);z++) {
                    Block* b =getBlock(x,y,z);
                    if (b && !b->passable) {
                        (*i)->setVelocity((*i)->getVelocity()*glm::vec3(1,0,1));
                        (*i)->setPosition((*i)->getPosition()*glm::vec3(1,0,1)+glm::vec3(0,y-box->getHeight()/2-.001f,0));
                    }
                }
            }
        }

        (*i)->translate(glm::vec3(0,1,0)*(*i)->getVelocity()*seconds);
        box = (BoundingBox*) (*i)->getCollisionShape();
        pos = box->getPosition();
        distance = (*i)->getVelocity()*seconds;

        for (int x = floor(pos.x-box->getWidth()/2)-1; x+1 > pos.x-box->getWidth()/2 + distance.x; x--) {
            for (int y = floor(pos.y-box->getHeight()/2); y <= floor(pos.y+box->getHeight()/2);y++) {
                for (int z = floor(pos.z-box->getDepth()/2); z <= floor(pos.z+box->getDepth()/2);z++) {
                    Block* b = getBlock(x,y,z);
                    if (b && !b->passable) {
                        (*i)->setVelocity((*i)->getVelocity()*glm::vec3(0,1,1));
                        (*i)->setPosition((*i)->getPosition()*glm::vec3(0,1,1)+glm::vec3(x+1+box->getWidth()/2+.001f,0,0));
                    }
                }
            }
        }
        for (int x = floor(pos.x+box->getWidth()/2)+1; x < (pos.x+box->getWidth()/2 + distance.x); x++) {
            for (int y = floor(pos.y-box->getHeight()/2); y < ceil(pos.y+box->getHeight()/2);y++) {
                for (int z = floor(pos.z-box->getDepth()/2); z < ceil(pos.z+box->getDepth()/2);z++) {
                    Block* b = getBlock(x,y,z);
                    if (b && !b->passable) {
                        (*i)->setVelocity((*i)->getVelocity()*glm::vec3(0,1,1));
                        (*i)->setPosition((*i)->getPosition()*glm::vec3(0,1,1)+glm::vec3(x-box->getWidth()/2-.001f,0,0));
                    }
                }
            }
        }

        (*i)->translate(glm::vec3(1,0,0)*(*i)->getVelocity()*seconds);
        box = (BoundingBox*) (*i)->getCollisionShape();
        pos = box->getPosition();
        distance = (*i)->getVelocity()*seconds;

        for (int z = floor(pos.z-box->getDepth()/2)-1; z+1 > pos.z-box->getDepth()/2 + distance.z; z--) {
            for (int y = floor(pos.y-box->getHeight()/2); y <= floor(pos.y+box->getHeight()/2);y++) {
                for (int x = floor(pos.x-box->getWidth()/2); x <= floor(pos.x+box->getWidth()/2);x++) {
                    Block* b = getBlock(x,y,z);
                    if (b && !b->passable) {
                        (*i)->setVelocity((*i)->getVelocity()*glm::vec3(1,1,0));
                        (*i)->setPosition((*i)->getPosition()*glm::vec3(1,1,0)+glm::vec3(0,0,z+1+box->getDepth()/2+.001f));
                    }
                }
            }
        }
        for (int z = floor(pos.z+box->getDepth()/2)+1; z < (pos.z+box->getDepth()/2 + distance.z); z++) {
            for (int y = floor(pos.y-box->getHeight()/2); y < ceil(pos.y+box->getHeight()/2);y++) {
                for (int x = floor(pos.x-box->getWidth()/2); x < ceil(pos.x+box->getWidth()/2);x++) {
                    Block* b = getBlock(x,y,z);
                    if (b && !b->passable) {
                        (*i)->setVelocity((*i)->getVelocity()*glm::vec3(1,1,0));
                        (*i)->setPosition((*i)->getPosition()*glm::vec3(1,1,0)+glm::vec3(0,0,z-box->getDepth()/2-.001f));
                    }
                }
            }
        }
        (*i)->translate(glm::vec3(0,0,1)*(*i)->getVelocity()*seconds);
    }
}

RayCollision VoxelManager::shootRay(const glm::vec3 &position, const glm::vec3 &direction, GLfloat length) {
    const int stepX = (direction.x > 0) ? 1 : -1;
    const int stepY = (direction.y > 0) ? 1 : -1;
    const int stepZ = (direction.z > 0) ? 1 : -1;

    const float tDeltaX = (direction.x != 0) ? fabs(1/direction.x) : 0;
    const float tDeltaY = (direction.y != 0) ? fabs(1/direction.y) : 0;
    const float tDeltaZ = (direction.z != 0) ? fabs(1/direction.z) : 0;

    int X = floor(position.x);
    int Y = floor(position.y);
    int Z = floor(position.z);

    float tMaxX = (direction.x != 0) ? fabs(X+((stepX == 1) ? 1: 0)-position.x)/fabs(direction.x) : -10; //negative means don't pick tMaxX
    float tMaxY = (direction.y != 0) ? fabs(Y+((stepY == 1) ? 1: 0)-position.y)/fabs(direction.y) : -10;
    float tMaxZ = (direction.z != 0) ? fabs(Z+((stepZ == 1) ? 1: 0)-position.z)/fabs(direction.z) : -10;

    GLfloat distance = 0;
    Block* b = getBlock(X, Y, Z);
    RayCollision r = {X,Y,Z,top,b};
    while (b != NULL && b->passable) {
        if(tMaxX > 0 && tMaxX < fabs(tMaxY)) {
            if(tMaxX > 0 && tMaxX < fabs(tMaxZ)) {
                distance = tMaxX;
                X= X + stepX;
                tMaxX= tMaxX + tDeltaX;
                r.dir = (stepX > 0) ? front: back;
                r.x = X;
            } else {
                distance = tMaxZ;
                Z= Z + stepZ;
                tMaxZ= tMaxZ + tDeltaZ;
                r.dir = (stepZ > 0) ? left: right;
                r.z = Z;
            }
        } else {
            if(tMaxY > 0 && tMaxY < fabs(tMaxZ)) {
                distance = tMaxY;
                Y= Y + stepY;
                tMaxY= tMaxY + tDeltaY;
                r.dir = (stepY > 0) ? bottom: top;
                r.y = Y;
            } else {
                distance = tMaxZ;
                Z= Z + stepZ;
                tMaxZ= tMaxZ + tDeltaZ;
                r.dir = (stepZ > 0) ? left: right;
                r.z = Z;
            }
        }
        if (distance > length) {
            r.b = NULL;
            return r;
        }
        b = getBlock(X,Y,Z);
        r.b = b;
    }
    return r;
}
