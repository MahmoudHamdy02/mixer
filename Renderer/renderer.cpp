#include "renderer.h"

#include <GL/gl.h>

#include <memory>

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

    grid = std::make_unique<Grid>();
}

void Renderer::resize(int width, int height)
{
    projection = pmp::perspective_matrix(45.0f, (float)width / height, 0.1f, 100.0f);
    shader->setMatrix4("projection", projection.data());
    grid->shader->setMatrix4("projection", projection.data());
    glViewport(0, 0, width, height);
}

void Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();

    // Set camera view matrix
    view = camera.GetViewMatrix();
    shader->setMatrix4("view", view.data());
    grid->shader->setMatrix4("view", view.data());

    for (MeshGL& mesh : meshGLs) {
        mesh.draw();
    }

    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    grid->render();
}

void Renderer::moveCamera(float offsetX, float offsetY)
{
    camera.processMouse(offsetX, offsetY);
}
