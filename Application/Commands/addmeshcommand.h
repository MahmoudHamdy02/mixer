#pragma once

#include <memory>

#include "Commands/command.h"
#include "Widgets/glwidget.h"
#include "meshgl.h"
#include "renderer.h"
#include "scenecontroller.h"

class AddMeshCommand : public Command
{
    GLWidget* glWidget;
    SceneController* scene;
    Renderer* renderer;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<MeshGL> meshGL;

public:
    AddMeshCommand(GLWidget* glWidget, SceneController* scene, Renderer* renderer, const std::shared_ptr<Mesh>& mesh)
        : glWidget(glWidget), scene(scene), renderer(renderer), mesh(mesh)
    {
        meshGL = std::make_shared<MeshGL>(mesh);
    }

    void execute() override
    {
        scene->addMesh(mesh);
        renderer->addMeshGL(meshGL);
        glWidget->update();
    }

    void undo() override
    {
        scene->deleteMesh(mesh);
        renderer->deleteMeshGL(meshGL);
        glWidget->update();
    }
};
