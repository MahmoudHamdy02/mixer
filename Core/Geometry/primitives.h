#pragma once

#include "pmp/surface_mesh.h"

class Primitives
{
public:
    enum class Type { Cube, Sphere };
    static void createCube(pmp::SurfaceMesh& mesh);
    static void createSphere(pmp::SurfaceMesh& mesh, int slices = 32, int stacks = 16);
};
