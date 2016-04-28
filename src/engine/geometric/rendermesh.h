#ifndef RENDERMESH
#define RENDERMESH

#include "engine/common/rendergeometry/obj.h"
#include "engine/common/rendergeometry/shape.h"

class RenderMesh : public Shape
{
public:
    RenderMesh(OBJ *o);
    ~RenderMesh();
};

#endif // RENDERMESH

