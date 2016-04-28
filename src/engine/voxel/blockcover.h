#ifndef BLOCKCOVER
#define BLOCKCOVER

#include "engine/common/rendergeometry/shape.h"
#include "engine/common/direction.h"

class BlockCover : public Shape
{
public:
    BlockCover(int x, int y, int z, Direction d, int texIndex, int texelDimensions);
    ~BlockCover();

    void draw();

private:
    inline glm::vec2 indexToTexel(unsigned char index, int texelDimensions);
};

#endif // BLOCKCOVER

