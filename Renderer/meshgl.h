#pragma once

#include <qopenglextrafunctions.h>

#include "mesh.h"
#include "pmp/surface_mesh.h"

class MeshGL : private QOpenGLExtraFunctions
{
    unsigned int VBO, VAO, EBO;
    unsigned int numIndices;
    pmp::SurfaceMesh meshGL;

    void setup(Mesh& mesh);

public:
    MeshGL(Mesh& mesh);
    void draw();
    void drawVertices();
};
