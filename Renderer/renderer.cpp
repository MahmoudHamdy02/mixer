#include "renderer.h"

#include <GL/gl.h>
#include <qopenglext.h>

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
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup shaders
    wireframeShader =
        new Shader(":/Renderer/Shaders/wireframeVertex.glsl", ":/Renderer/Shaders/wireframeFragment.glsl");
    flatShader = new Shader(":/Renderer/Shaders/flatVertex.glsl", ":/Renderer/Shaders/flatFragment.glsl");
    pointsShader = new Shader(":/Renderer/Shaders/pointsVertex.glsl", ":/Renderer/Shaders/pointsFragment.glsl");
    flatShader->use();

    // Setup model matrix
    // Projection is set in resize() as it is called automatically on startup
    model = pmp::mat4::identity();
    // TODO: Figure this out
    flatShader->setMatrix4("model", model.data());
    wireframeShader->setMatrix4("model", model.data());
    pointsShader->setMatrix4("model", model.data());

    Mesh mesh = scene->getMeshes()[0];
    meshGLs.push_back(MeshGL(mesh));

    grid = std::make_unique<Grid>();
}

void Renderer::resize(int width, int height)
{
    // TODO: Bind near and far values to grid fragment shader
    projection = pmp::perspective_matrix(45.0f, (float)width / height, 0.1f, 200.0f);
    flatShader->setMatrix4("projection", projection.data());
    wireframeShader->setMatrix4("projection", projection.data());
    pointsShader->setMatrix4("projection", projection.data());
    grid->shader->setMatrix4("projection", projection.data());
    glViewport(0, 0, width, height);
}

void Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if (renderMode == RenderMode::Wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        wireframeShader->use();
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        flatShader->use();
    }

    // Set camera view matrix
    view = camera.getViewMatrix();
    flatShader->setMatrix4("view", view.data());
    wireframeShader->setMatrix4("view", view.data());
    pointsShader->setMatrix4("view", view.data());
    grid->shader->setMatrix4("view", view.data());
    flatShader->setVec3("cameraDirection", camera.front);
    pointsShader->setVec3("cameraDirection", camera.front);

    for (MeshGL& mesh : meshGLs) {
        mesh.draw();
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    grid->render();

    glDisable(GL_DEPTH_TEST);
    pointsShader->use();
    for (MeshGL& mesh : meshGLs) {
        mesh.drawVertices();
    }
}

void Renderer::setRenderMode(RenderMode mode)
{
    renderMode = mode;
}

void Renderer::moveCamera(float offsetX, float offsetY)
{
    camera.processMouse(offsetX, offsetY);
}

void Renderer::panCamera(float offsetX, float offsetY)
{
    camera.processMove(offsetX, offsetY);
}

void Renderer::zoomCamera(float distance)
{
    camera.addDistance(distance);
}
