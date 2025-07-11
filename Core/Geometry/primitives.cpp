#include "primitives.h"

#include <vector>

#include "pmp/algorithms/normals.h"
#include "pmp/algorithms/shapes.h"
#include "pmp/algorithms/triangulation.h"
#include "pmp/surface_mesh.h"
#include "pmp/types.h"

void Primitives::createCube(pmp::SurfaceMesh& mesh)
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

    std::vector<pmp::Vertex> v;
    for (const auto& p : vertices) v.push_back(mesh.add_vertex(p));

    // Define 12 triangles (2 per cube face)
    std::vector<std::array<int, 3>> triangles = {{0, 3, 1}, {3, 2, 1}, {1, 2, 5}, {2, 6, 5}, {5, 6, 4}, {6, 7, 4},
                                                 {4, 7, 0}, {7, 3, 0}, {3, 7, 2}, {7, 6, 2}, {4, 0, 5}, {0, 1, 5}};

    for (const auto& tri : triangles) {
        mesh.add_face({v[tri[0]], v[tri[1]], v[tri[2]]});
    }

    pmp::face_normals(mesh);
}

void Primitives::createSphere(pmp::SurfaceMesh& mesh, int slices, int stacks)
{
    mesh = pmp::uv_sphere(pmp::Point(0, 0, 0), 1.0, slices, stacks);
    pmp::triangulate(mesh);
    pmp::face_normals(mesh);
}
