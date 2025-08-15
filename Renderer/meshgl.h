#pragma once

#include <qopenglextrafunctions.h>

#include <memory>

#include "mesh.h"

class MeshGL : private QOpenGLExtraFunctions
{
    unsigned int VBO, VAO, EBO;
    unsigned int numIndices;

    unsigned int instancedVAO, instancedVBO, instancedEBO, instancedDataVBO;
    unsigned int instancedNumIndices;

    void setup(const std::shared_ptr<Mesh>& mesh);

public:
    std::shared_ptr<Mesh> mesh;

    // TODO: Add destructor that frees buffers
    MeshGL(const std::shared_ptr<Mesh>& mesh);

    void updateBuffers();

    void draw();
    void drawVertices();
};
