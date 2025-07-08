#pragma once

#include <qopenglextrafunctions.h>

#include "mesh.h"

class MeshGL : private QOpenGLExtraFunctions
{
    unsigned int VBO, VAO, EBO;
    unsigned int numIndices;

    void setup(Mesh& mesh);

public:
    MeshGL(Mesh& mesh);
    void draw();
};
