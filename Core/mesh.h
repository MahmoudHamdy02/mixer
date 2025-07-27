#pragma once

#include <string>
#include <vector>

#include "pmp/bounding_box.h"
#include "pmp/mat_vec.h"
#include "pmp/surface_mesh.h"
#include "pmp/types.h"

class Mesh
{
    // Uniquely identifies object
    std::string name;
    pmp::SurfaceMesh surfaceMesh;
    pmp::Point center;
    // Axis Aligned Bounding Box
    pmp::BoundingBox aabb;

public:
    Mesh(std::string name);

    const std::string& getName() const;
    const pmp::SurfaceMesh& getSurfaceMesh() const;
    const pmp::BoundingBox& getAABB() const;

    void translate(pmp::vec3 vec);

    void setSelectedVertices(const std::vector<pmp::Vertex>& vertices);
    void unselectVertices();
};
