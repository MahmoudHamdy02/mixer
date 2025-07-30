#include "meshgl.h"

#include <vector>

#include "mesh.h"
#include "pmp/algorithms/normals.h"
#include "pmp/algorithms/triangulation.h"
#include "pmp/surface_mesh.h"

void MeshGL::setup(Mesh* mesh)
{
    this->mesh = mesh;
    const pmp::SurfaceMesh& surfaceMesh = mesh->getSurfaceMesh();
    pmp::SurfaceMesh meshGL = surfaceMesh;
    pmp::triangulate(meshGL);
    pmp::face_normals(meshGL);

    auto vpos = meshGL.vertex_property<pmp::Point>("v:point");
    auto vselected = meshGL.vertex_property<float>("v:selected");
    auto fnormal = meshGL.face_property<pmp::Normal>("f:normal");

    // Stores position and normals
    std::vector<float> vertices = {};
    std::vector<unsigned int> indices = {};

    // Instanced vertex handles, stores position and if it is selected
    std::vector<float> instancedVertices;

    unsigned int index = 0;
    for (auto f : meshGL.faces()) {
        pmp::Normal n = fnormal[f];
        std::vector<unsigned int> faceIndices;

        for (auto v : meshGL.vertices(f)) {
            pmp::Point p = vpos[v];

            vertices.push_back(p[0]);
            vertices.push_back(p[1]);
            vertices.push_back(p[2]);

            vertices.push_back(n[0]);
            vertices.push_back(n[1]);
            vertices.push_back(n[2]);

            instancedVertices.push_back(p[0]);
            instancedVertices.push_back(p[1]);
            instancedVertices.push_back(p[2]);
            instancedVertices.push_back(n[0]);
            instancedVertices.push_back(n[1]);
            instancedVertices.push_back(n[2]);
            instancedVertices.push_back(vselected[v]);

            faceIndices.push_back(index++);
        }

        if (faceIndices.size() == 3) {
            indices.push_back(faceIndices[0]);
            indices.push_back(faceIndices[1]);
            indices.push_back(faceIndices[2]);
        }
    }
    numIndices = indices.size();
    instancedNumIndices = meshGL.n_faces() * 3;

    // TODO: Render as spheres?
    // Setup vertex handle instance
    float instanceCube[] = {
        -0.01f, -0.01f, -0.01f,  // 0
        0.01f,  -0.01f, -0.01f,  // 1
        0.01f,  0.01f,  -0.01f,  // 2
        -0.01f, 0.01f,  -0.01f,  // 3
        -0.01f, -0.01f, 0.01f,   // 4
        0.01f,  -0.01f, 0.01f,   // 5
        0.01f,  0.01f,  0.01f,   // 6
        -0.01f, 0.01f,  0.01f    // 7
    };

    unsigned int triangles[] = {0, 3, 1, 3, 2, 1, 1, 2, 5, 2, 6, 5, 5, 6, 4, 6, 7, 4,
                                4, 7, 0, 7, 3, 0, 3, 7, 2, 7, 6, 2, 4, 0, 5, 0, 1, 5};

    glGenVertexArrays(1, &instancedVAO);
    glGenBuffers(1, &instancedVBO);
    glGenBuffers(1, &instancedEBO);
    glGenBuffers(1, &instancedDataVBO);

    glBindVertexArray(instancedVAO);

    // Vertices VBO
    glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), instanceCube, GL_STATIC_DRAW);

    // Object Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Element Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instancedEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), triangles, GL_STATIC_DRAW);

    // Instance data
    glBindBuffer(GL_ARRAY_BUFFER, instancedDataVBO);
    glBufferData(GL_ARRAY_BUFFER, instancedVertices.size() * sizeof(float), instancedVertices.data(), GL_STATIC_DRAW);

    // Instance Position attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    // Instance Normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    // Instance Selected attribute
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    // Object setup
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertices VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // --- Element Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void MeshGL::updateBuffers()
{
    const pmp::SurfaceMesh& surfaceMesh = mesh->getSurfaceMesh();
    pmp::SurfaceMesh meshGL = surfaceMesh;
    pmp::triangulate(meshGL);
    pmp::face_normals(meshGL);
    auto vpos = meshGL.get_vertex_property<pmp::Point>("v:point");
    auto vselected = meshGL.get_vertex_property<float>("v:selected");
    auto fnormal = meshGL.get_face_property<pmp::Normal>("f:normal");

    // Stores position and normals
    std::vector<float> vertices = {};
    std::vector<unsigned int> indices = {};

    // Instanced vertex handles, stores position and if it is selected
    std::vector<float> instancedVertices;

    unsigned int index = 0;
    for (auto f : meshGL.faces()) {
        pmp::Normal n = fnormal[f];
        std::vector<unsigned int> faceIndices;

        for (auto v : meshGL.vertices(f)) {
            pmp::Point p = vpos[v];

            vertices.push_back(p[0]);
            vertices.push_back(p[1]);
            vertices.push_back(p[2]);

            vertices.push_back(n[0]);
            vertices.push_back(n[1]);
            vertices.push_back(n[2]);

            instancedVertices.push_back(p[0]);
            instancedVertices.push_back(p[1]);
            instancedVertices.push_back(p[2]);
            instancedVertices.push_back(n[0]);
            instancedVertices.push_back(n[1]);
            instancedVertices.push_back(n[2]);
            instancedVertices.push_back(vselected[v]);

            faceIndices.push_back(index++);
        }

        if (faceIndices.size() == 3) {
            indices.push_back(faceIndices[0]);
            indices.push_back(faceIndices[1]);
            indices.push_back(faceIndices[2]);
        }
    }
    numIndices = indices.size();
    instancedNumIndices = meshGL.n_faces() * 3;

    glBindVertexArray(VAO);
    // Vertices VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Element Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Instance buffer
    glBindBuffer(GL_ARRAY_BUFFER, instancedDataVBO);
    glBufferData(GL_ARRAY_BUFFER, instancedVertices.size() * sizeof(float), instancedVertices.data(), GL_STATIC_DRAW);
}

MeshGL::MeshGL(Mesh* mesh)
{
    initializeOpenGLFunctions();
    setup(mesh);
}

void MeshGL::draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void MeshGL::drawVertices()
{
    glBindVertexArray(instancedVAO);
    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, instancedNumIndices);
    glBindVertexArray(0);
}
