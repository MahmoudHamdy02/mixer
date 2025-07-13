#pragma once

#include <qopenglextrafunctions.h>

#include "mesh.h"
#include "pmp/surface_mesh.h"

class MeshGL : private QOpenGLExtraFunctions
{
    unsigned int VBO, VAO, EBO;
    unsigned int numIndices;

    void setup(Mesh& mesh);

public:
    pmp::SurfaceMesh meshGL;
    MeshGL(Mesh& mesh);

    void updateBuffers();

    void draw();
    void drawVertices();
};
