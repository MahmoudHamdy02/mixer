#include "renderer.h"

#include <GL/gl.h>

#include "mesh.h"
#include "pmp/mat_vec.h"
#include "shader.h"

Renderer::Renderer(SceneController* scene) : scene(scene) {}

void Renderer::initialize()
{
    initializeOpenGLFunctions();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Setup shaders
    shader = new Shader(":/Renderer/Shaders/vertex.glsl", ":/Renderer/Shaders/fragment.glsl");
    shader->use();

    // Setup model and view matrices
    // Projection is set in resize() as it is called automatically on startup
    model = pmp::mat4::identity();
    shader->setMatrix4("model", model.data());

    Mesh mesh = scene->getMeshes()[0];
    meshGLs.push_back(MeshGL(mesh));

    setupGrid();
}

void Renderer::resize(int width, int height)
{
    projection = pmp::perspective_matrix(45.0f, (float)width / height, 0.1f, 100.0f);
    shader->setMatrix4("projection", projection.data());
    grid.shader->setMatrix4("projection", projection.data());
    glViewport(0, 0, width, height);
}

void Renderer::setupGrid()
{
    float vertices[] = {1.0f,  1.0f,  0.0f, -1.0f, -1.0f, 0.0f, -1.0f, 1.0f,  0.0f,
                        -1.0f, -1.0f, 0.0f, 1.0f,  1.0f,  0.0f, 1.0f,  -1.0f, 0.0f};

    // Vertex Array Object + Vertex Buffer Object
    glGenVertexArrays(1, &grid.VAO);
    glGenBuffers(1, &grid.VBO);

    glBindVertexArray(grid.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, grid.VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    grid.shader = new Shader(":/Renderer/Shaders/gridVertex.glsl", ":/Renderer/Shaders/gridFragment.glsl");
}

void Renderer::renderGrid()
{
    glBindVertexArray(grid.VAO);
    grid.shader->use();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();

    // Set camera view matrix
    view = camera.GetViewMatrix();
    shader->setMatrix4("view", view.data());
    grid.shader->setMatrix4("view", view.data());

    for (MeshGL& mesh : meshGLs) {
        mesh.draw();
    }

    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    renderGrid();
}

void Renderer::moveCamera(float offsetX, float offsetY)
{
    camera.processMouse(offsetX, offsetY);
}
