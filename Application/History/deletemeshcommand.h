#pragma once

#include <memory>

#include "History/command.h"
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
    DeleteMeshCommand(GLWidget* glWidget, SceneController* scene, Renderer* renderer, std::shared_ptr<Mesh> mesh,
                      std::shared_ptr<MeshGL> meshGL)
        : glWidget(glWidget), scene(scene), renderer(renderer), mesh(mesh), meshGL(meshGL)
    {
    }

    void execute() override
    {
        scene->deleteMesh(mesh);
        renderer->deleteMeshGL(meshGL);

        glWidget->update();
    }

    void undo() override
    {
        scene->addMesh(mesh);
        renderer->addMeshGL(meshGL);

        glWidget->update();
    }
};
