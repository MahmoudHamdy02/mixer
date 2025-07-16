#pragma once

#include <string>
#include <vector>

#include "pmp/surface_mesh.h"

class Mesh
{
    // Uniquely identifies object
    std::string name;
    pmp::SurfaceMesh surfaceMesh;

public:
    Mesh();

    const std::string& getName() const;
    const pmp::SurfaceMesh& getSurfaceMesh() const;

    void setSelectedVertices(const std::vector<pmp::Vertex>& vertices);
};
