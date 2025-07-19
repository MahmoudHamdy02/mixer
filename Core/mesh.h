#pragma once

#include <string>
#include <vector>

#include "pmp/mat_vec.h"
#include "pmp/surface_mesh.h"

class Mesh
{
    // Uniquely identifies object
    std::string name;
    pmp::SurfaceMesh surfaceMesh;

public:
    Mesh(std::string name);

    const std::string& getName() const;
    const pmp::SurfaceMesh& getSurfaceMesh() const;

    void translate(pmp::vec3 vec);

    void setSelectedVertices(const std::vector<pmp::Vertex>& vertices);
    void unselectVertices();
};
