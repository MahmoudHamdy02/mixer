#include "renderer.h"

#include <iostream>
#include <memory>
#include <vector>

#include "mesh.h"
#include "meshgl.h"
#include "pmp/mat_vec.h"
#include "scenecontroller.h"
#include "selectionmanager.h"
#include "selectionrectangle.h"
#include "shader.h"
#include "toolmanager.h"

Renderer::Renderer(SceneController* scene, SelectionManager* selectionManager)
    : scene(scene), selectionManager(selectionManager)
{
    connect(scene, &SceneController::onMeshDeleted, this, &Renderer::deleteMesh);
}

void Renderer::initialize()
{
    initializeOpenGLFunctions();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup shaders
    wireframeShader = std::make_unique<Shader>(":/wireframeVertex.glsl", ":/wireframeFragment.glsl");
    flatShader = std::make_unique<Shader>(":/flatVertex.glsl", ":/flatFragment.glsl");
    outlineShader = std::make_unique<Shader>(":/outlineVertex.glsl", ":/outlineFragment.glsl");
    pointsShader = std::make_unique<Shader>(":/pointsVertex.glsl", ":/pointsFragment.glsl");
    flatShader->use();

    // Setup model matrix
    // Projection is set in resize() as it is called automatically on startup
    model = pmp::mat4::identity();
    // TODO: Figure this out
    flatShader->setMatrix4("model", model.data());
    wireframeShader->setMatrix4("model", model.data());
    pointsShader->setMatrix4("model", model.data());
    outlineShader->setMatrix4("model", model.data());

    std::vector<Mesh>& meshes = scene->getMeshes();
    for (Mesh& mesh : meshes) {
        meshGLs.emplace_back(mesh);
    }

    grid = std::make_unique<Grid>();
    selectionRectangle = std::make_unique<SelectionRectangle>();
}

void Renderer::resize(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    // TODO: Bind near and far values to grid fragment shader
    projection = pmp::perspective_matrix(45.0f, (float)width / height, 0.1f, 200.0f);
    flatShader->setMatrix4("projection", projection.data());
    outlineShader->setMatrix4("projection", projection.data());
    wireframeShader->setMatrix4("projection", projection.data());
    pointsShader->setMatrix4("projection", projection.data());
    grid->shader->setMatrix4("projection", projection.data());
    glViewport(0, 0, width, height);
}

void Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Set camera view matrix
    view = camera.getViewMatrix();
    flatShader->setMatrix4("view", view.data());
    outlineShader->setMatrix4("view", view.data());
    wireframeShader->setMatrix4("view", view.data());
    pointsShader->setMatrix4("view", view.data());
    grid->shader->setMatrix4("view", view.data());
    flatShader->setVec3("cameraDirection", camera.front);
    pointsShader->setVec3("cameraDirection", camera.front);

    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);  // All fragments should write to the stencil buffer

    // Scene meshes
    for (MeshGL& mesh : meshGLs) {
        drawMesh(mesh, selectionManager->isMeshSelected(mesh.mesh));
    }

    // Floor grid
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    grid->render();

    // TODO: Fix handles rendering through other objects
    if (ToolManager::selectedEditMode == ToolManager::EditMode::Vertex) {
        // Vertex handles
        glDisable(GL_DEPTH_TEST);
        pointsShader->use();
        for (MeshGL& mesh : meshGLs) {
            mesh.drawVertices();
        }
    }

    // Selection rectangle
    glEnable(GL_DEPTH_TEST);
    selectionRectangle->render();

    // Workaround for Qt transparency issues on Wayland: set alpha manually to 1.0f at the end of the render loop
    // See: https://github.com/FreeCAD/FreeCAD/pull/19499
    glColorMask(false, false, false, true);
    glClear(GL_COLOR_BUFFER_BIT);
    glColorMask(true, true, true, true);
}

void Renderer::drawMesh(MeshGL& mesh, bool outlined)
{
    std::cout << mesh.mesh->getName() << " " << outlined << std::endl;
    if (ToolManager::selectedRenderMode == ToolManager::RenderMode::Wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        wireframeShader->use();
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        flatShader->use();
    }

    if (!outlined) {
        mesh.draw();
    } else {
        glStencilMask(0xFF);
        glClear(GL_STENCIL_BUFFER_BIT);
        mesh.draw();

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);  // Only draw where stencil != 1
        glStencilMask(0x00);
        glDepthFunc(GL_ALWAYS);  // Draw outline on top

        outlineShader->use();
        pmp::Point center = mesh.mesh->getCenter();
        pmp::mat4 outlineModel = pmp::translation_matrix(-center);
        outlineModel = pmp::scaling_matrix(1.025f) * outlineModel;
        outlineModel = pmp::translation_matrix(center) * outlineModel;
        outlineShader->setMatrix4("model", outlineModel.data());
        mesh.draw();

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glDepthFunc(GL_LESS);
    }
}

Ray Renderer::mouseToWorldRay(float mouseX, float mouseY) const
{
    float ndcX = (2.0f * mouseX) / screenWidth - 1.0f;
    float ndcY = 1.0f - (2.0f * mouseY) / screenHeight;

    pmp::vec4 clipPos = pmp::vec4(ndcX, ndcY, 1.0f, 1.0f);

    pmp::vec4 worldPos = pmp::inverse(projection * view) * clipPos;
    pmp::vec3 pos = pmp::vec3(worldPos[0] / worldPos[3], worldPos[1] / worldPos[3], worldPos[2] / worldPos[3]);

    pmp::vec3 direction = pmp::normalize(pos);
    pmp::vec3 origin = camera.position;

    return Ray(origin, direction);
}

void Renderer::updateMesh(const std::string& name)
{
    for (MeshGL& meshGL : meshGLs) {
        if (meshGL.mesh->getName() == name) {
            meshGL.updateBuffers();
            break;
        }
    }
}

void Renderer::updateMeshes()
{
    for (MeshGL& meshGL : meshGLs) {
        meshGL.updateBuffers();
    }
}

void Renderer::deleteMesh(Mesh* mesh)
{
    std::cout << "MeshGL delete: " << mesh->getName() << std::endl;
    auto it = std::find_if(meshGLs.begin(), meshGLs.end(), [&](MeshGL& elem) { return (&elem)->mesh == mesh; });

    if (it != meshGLs.end()) {
        meshGLs.erase(it);
        std::cout << "MeshGL deleted" << std::endl;
    }
    std::cout << "MeshGLs: " << meshGLs.size() << " " << meshGLs[0].mesh->getAABB().center() << std::endl;
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

void Renderer::setSelectionRectangleVertices(const pmp::vec2& min, const pmp::vec2& max)
{
    selectionRectangle->setVertices(min, max);
}

const pmp::mat4& Renderer::getModelMatrix() const
{
    return model;
}

const pmp::mat4& Renderer::getViewMatrix() const
{
    return view;
}

const pmp::mat4& Renderer::getProjectionMatrix() const
{
    return projection;
}

const pmp::mat4 Renderer::getMVPMatrix() const
{
    return projection * view * model;
}

const Camera& Renderer::getCamera() const
{
    return camera;
}
