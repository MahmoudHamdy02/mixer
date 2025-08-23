#include "mesh.h"

#include <string>
#include <vector>

#include "Geometry/primitives.h"
#include "pmp/algorithms/utilities.h"
#include "pmp/bounding_box.h"
#include "pmp/mat_vec.h"
#include "pmp/surface_mesh.h"

Mesh::Mesh(Primitives::Type type, std::string name) : name(name)
{
    switch (type) {
        case Primitives::Type::Cube:
            Primitives::createCube(surfaceMesh);
            break;
        case Primitives::Type::Sphere:
            Primitives::createSphere(surfaceMesh, 32, 16);
            break;
    }

    aabb = pmp::bounds(surfaceMesh);
    center = aabb.center();
}

const std::string& Mesh::getName() const
{
    return name;
}

pmp::SurfaceMesh& Mesh::getSurfaceMesh()
{
    return surfaceMesh;
}

const pmp::SurfaceMesh& Mesh::getSurfaceMesh() const
{
    return surfaceMesh;
}

const pmp::BoundingBox& Mesh::getAABB() const
{
    return aabb;
}

const pmp::Point& Mesh::getCenter() const
{
    return center;
}

void Mesh::translate(pmp::vec3 vec)
{
    for (pmp::Vertex v : surfaceMesh.vertices()) {
        surfaceMesh.position(v) += vec;
    }
    aabb = pmp::bounds(surfaceMesh);
    center = aabb.center();
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

void Mesh::unselectVertices()
{
    auto selected = surfaceMesh.get_vertex_property<float>("v:selected");

    for (const pmp::Vertex& v : surfaceMesh.vertices()) {
        selected[v] = 0.0f;
    }
}

void Mesh::deleteSelectedVertices()
{
    auto selected = surfaceMesh.get_vertex_property<float>("v:selected");
    for (pmp::Vertex v : surfaceMesh.vertices()) {
        if (selected[v] > 0.0f)
            surfaceMesh.delete_vertex(v);
    }
    surfaceMesh.garbage_collection();
}
