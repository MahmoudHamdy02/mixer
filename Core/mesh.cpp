#include "mesh.h"

#include "Geometry/primitives.h"
#include "pmp/surface_mesh.h"

Mesh::Mesh()
{
    // Primitives::createCube(surfaceMesh);
    Primitives::createSphere(surfaceMesh, 32, 16);
}

const pmp::SurfaceMesh& Mesh::getSurfaceMesh() const
{
    return surfaceMesh;
}
