#pragma once

#include "pmp/surface_mesh.h"

class Primitives
{
public:
    static void createCube(pmp::SurfaceMesh& mesh);
    static void createSphere(pmp::SurfaceMesh& mesh, int slices = 32, int stacks = 16);
};
