#pragma once

#include "pmp/surface_mesh.h"

class Mesh
{
    pmp::SurfaceMesh surfaceMesh;

public:
    Mesh();
    const pmp::SurfaceMesh& getSurfaceMesh() const;
};
