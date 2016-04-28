#ifndef SKYBOX
#define SKYBOX

#include "engine/common/rendergeometry/shape.h"

class Skybox : public Shape
{
public:
    Skybox();
    ~Skybox();

    void draw();

    void rebuild(int x, int y, int z);
};

#endif // SKYBOX

