#pragma once

#include <qopenglextrafunctions.h>

#include "mesh.h"

class MeshGL : private QOpenGLExtraFunctions
{
    unsigned int VBO, VAO, EBO;
    unsigned int numIndices;

    unsigned int instancedVAO, instancedVBO, instancedEBO, instancedDataVBO;
    unsigned int instancedNumIndices;

    void setup(Mesh* mesh);

public:
    Mesh* mesh;

    // TODO: Add destructor that frees buffers
    MeshGL(Mesh* mesh);

    void updateBuffers();

    void draw();
    void drawVertices();
};
