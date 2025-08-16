#pragma once

#include <memory>

#include "Commands/command.h"
#include "Widgets/glwidget.h"
#include "mesh.h"
#include "meshgl.h"
#include "renderer.h"
#include "scenecontroller.h"

class DeleteMeshCommand : public Command
{
    GLWidget* glWidget;
    SceneController* scene;
    Renderer* renderer;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<MeshGL> meshGL;

public:
    DeleteMeshCommand(GLWidget* glWidget, SceneController* scene, Renderer* renderer, std::shared_ptr<Mesh> mesh)
        : glWidget(glWidget), scene(scene), renderer(renderer), mesh(mesh)
    {
        meshGL = renderer->getMeshGLFromMesh(mesh);
    }

    void execute() override
    {
        scene->deleteMesh(mesh);
        if (meshGL)
            renderer->deleteMeshGL(meshGL);

        glWidget->update();
    }

    void undo() override
    {
        scene->addMesh(mesh);
        if (meshGL)
            renderer->addMeshGL(meshGL);

        glWidget->update();
    }
};
