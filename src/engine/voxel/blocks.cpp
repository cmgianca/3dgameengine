#include "engine/voxel/blocks.h"

Blocks::Blocks() :
    m_blocks(new Block[256])
{
}

Blocks::~Blocks(){
    delete m_blocks;
}

Block* Blocks::getBlock(unsigned char type) {
    return &m_blocks[type];
}

int Blocks::getTexelDimensions(){
    return m_texelDimensions;
}
