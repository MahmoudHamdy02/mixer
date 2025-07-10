#include "mesh.h"

#include "Geometry/primitives.h"

Mesh::Mesh()
{
    Primitives::createCube(surfaceMesh);
}

pmp::SurfaceMesh& Mesh::getSurfaceMesh()
{
    return surfaceMesh;
}
