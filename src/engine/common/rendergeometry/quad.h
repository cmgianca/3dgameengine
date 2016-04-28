#ifndef QUAD
#define QUAD

#include "engine/common/rendergeometry/shape.h"

/**
 * @brief The Quad class
 */
class Quad : public Shape
{
public:
    Quad();
    ~Quad();

    void  draw();
};

#endif // QUAD
