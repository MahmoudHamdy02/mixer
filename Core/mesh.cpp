#include "mesh.h"

#include "pmp/algorithms/normals.h"
#include "pmp/types.h"

Mesh::Mesh()
{
    // Define 8 cube corners
    std::vector<pmp::Point> vertices = {
        pmp::Point(-1, -1, -1),  // 0
        pmp::Point(1, -1, -1),   // 1
        pmp::Point(1, 1, -1),    // 2
        pmp::Point(-1, 1, -1),   // 3
        pmp::Point(-1, -1, 1),   // 4
        pmp::Point(1, -1, 1),    // 5
        pmp::Point(1, 1, 1),     // 6
        pmp::Point(-1, 1, 1)     // 7
    };

    // Add vertices to the mesh
    std::vector<pmp::Vertex> v;
    for (const auto& p : vertices) v.push_back(surfaceMesh.add_vertex(p));

    // Define 12 triangles (2 per cube face)
    std::vector<std::array<int, 3>> triangles = {// bottom
                                                 {0, 1, 2},
                                                 {0, 2, 3},
                                                 // top
                                                 {4, 6, 5},
                                                 {4, 7, 6},
                                                 // front
                                                 {0, 5, 1},
                                                 {0, 4, 5},
                                                 // right
                                                 {1, 6, 2},
                                                 {1, 5, 6},
                                                 // back
                                                 {2, 7, 3},
                                                 {2, 6, 7},
                                                 // left
                                                 {3, 4, 0},
                                                 {3, 7, 4}};

    // Add triangles to the mesh
    for (const auto& tri : triangles) {
        surfaceMesh.add_face({v[tri[0]], v[tri[1]], v[tri[2]]});
    }
    pmp::face_normals(surfaceMesh);
}

pmp::SurfaceMesh& Mesh::getSurfaceMesh()
{
    return surfaceMesh;
}
