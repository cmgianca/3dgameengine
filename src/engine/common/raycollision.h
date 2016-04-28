#ifndef RAYCOLLISION
#define RAYCOLLISION

#include "engine/common/direction.h"
#include "engine/voxel/blocks.h"

struct RayCollision {
    int x,y,z;
    Direction dir;
    Block* b;
};

#endif // RAYCOLLISION

