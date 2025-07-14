#include "mesh.h"

#include <vector>

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

void Mesh::setSelectedVertices(const std::vector<pmp::Vertex>& vertices)
{
    auto selected = surfaceMesh.get_vertex_property<float>("v:selected");

    for (const pmp::Vertex& v : surfaceMesh.vertices()) {
        selected[v] = 0.0f;
    }

    for (const pmp::Vertex& v : vertices) {
        selected[v] = 1.0f;
    }
}
