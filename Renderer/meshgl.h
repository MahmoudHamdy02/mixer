#pragma once

#include <qopenglextrafunctions.h>

#include <memory>

#include "mesh.h"

class MeshGL : private QOpenGLExtraFunctions
{
    bool gpuInitalized = false;

    unsigned int VBO = 0, VAO = 0, EBO = 0;
    unsigned int numIndices = 0;

    unsigned int instancedVAO = 0, instancedVBO = 0, instancedEBO = 0, instancedDataVBO = 0;
    unsigned int instancedNumIndices = 0;

    void setup();

public:
    std::shared_ptr<Mesh> mesh;

    MeshGL(const std::shared_ptr<Mesh>& mesh);

    MeshGL(const MeshGL& other) = delete;
    MeshGL& operator=(const MeshGL& other) = delete;

    MeshGL(MeshGL&& other);
    MeshGL& operator=(MeshGL&& other);
    ~MeshGL();

    void updateBuffers();

    void draw();
    void drawVertices();
};
