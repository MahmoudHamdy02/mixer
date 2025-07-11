#include "meshgl.h"

#include "mesh.h"
#include "pmp/algorithms/normals.h"
#include "pmp/algorithms/triangulation.h"
#include "pmp/surface_mesh.h"

void MeshGL::setup(Mesh& mesh)
{
    meshGL = mesh.getSurfaceMesh();
    pmp::triangulate(meshGL);
    pmp::face_normals(meshGL);

    auto vpos = meshGL.vertex_property<pmp::Point>("v:point");
    auto fnormal = meshGL.face_property<pmp::Normal>("f:normal");

    // Stores position and normals
    std::vector<float> vertices = {};
    std::vector<unsigned int> indices = {};

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

            faceIndices.push_back(index++);
        }

        if (faceIndices.size() == 3) {
            indices.push_back(faceIndices[0]);
            indices.push_back(faceIndices[1]);
            indices.push_back(faceIndices[2]);
        }
    }
    numIndices = indices.size();

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

MeshGL::MeshGL(Mesh& mesh)
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
    glBindVertexArray(VAO);
    glDrawElements(GL_POINTS, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
