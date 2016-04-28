#include "game/worldblocks.h"

WorldBlocks::WorldBlocks() :
    Blocks()
{
    m_texture = "terrain";
    m_texelDimensions = 16;
    m_blocks[0] = {
        "empty",
        255,255,255,255,255,255,
        true,
        true

    };
    m_blocks[1] = {
        "grass",
        0,2,3,3,3,3,
        false,
        false
    };
    m_blocks[2] = {
        "stone",
        1,1,1,1,1,1,
        false,
        false
    };
    m_blocks[3] = {
        "dirt",
        2,2,2,2,2,2,
        false,
        false
    };
    m_blocks[4] = {
        "wood",
        4,4,4,4,4,4,
        false,
        false
    };
}

WorldBlocks::~WorldBlocks() {
}
