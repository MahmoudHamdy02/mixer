#include "mesh.h"

#include <iostream>
#include <string>
#include <vector>

#include "Geometry/primitives.h"
#include "pmp/algorithms/utilities.h"
#include "pmp/bounding_box.h"
#include "pmp/mat_vec.h"
#include "pmp/surface_mesh.h"

Mesh::Mesh(std::string name) : name(name)
{
    // Primitives::createCube(surfaceMesh);
    Primitives::createSphere(surfaceMesh, 32, 16);

    aabb = pmp::bounds(surfaceMesh);
    center = aabb.center();
}

Mesh::~Mesh()
{
    std::cout << "Mesh deleted: " << name << std::endl;
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
