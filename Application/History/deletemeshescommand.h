#pragma once

#include <memory>
#include <vector>

#include "History/command.h"
#include "Widgets/glwidget.h"
#include "mesh.h"
#include "renderer.h"
#include "scenecontroller.h"

class DeleteMeshesCommand : public Command
{
    GLWidget* glWidget;
    SceneController* scene;
    Renderer* renderer;
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::vector<std::shared_ptr<MeshGL>> meshGLs;

public:
    DeleteMeshesCommand(GLWidget* glWidget, SceneController* scene, Renderer* renderer,
                        const std::vector<std::shared_ptr<Mesh>>& meshes)
        : glWidget(glWidget), scene(scene), renderer(renderer), meshes(meshes)
    {
        for (const std::shared_ptr<Mesh>& mesh : meshes) {
            const std::shared_ptr<MeshGL> meshGL = renderer->getMeshGLFromMesh(mesh);
            if (meshGL)
                meshGLs.push_back(meshGL);
        }
    }

    void execute() override
    {
        for (const std::shared_ptr<Mesh>& mesh : meshes) scene->deleteMesh(mesh);
        for (const std::shared_ptr<MeshGL>& meshGL : meshGLs) renderer->deleteMeshGL(meshGL);
        glWidget->update();
    }

    void undo() override
    {
        for (const std::shared_ptr<Mesh>& mesh : meshes) scene->addMesh(mesh);
        for (const std::shared_ptr<MeshGL>& meshGL : meshGLs) renderer->addMeshGL(meshGL);
        glWidget->update();
    }
};
