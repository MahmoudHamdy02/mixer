#include "scenecontroller.h"

#include <memory>
#include <vector>

#include "mesh.h"

SceneController::SceneController()
{
}

const std::vector<std::shared_ptr<Mesh>>& SceneController::getMeshes() const
{
    return meshes;
}

void SceneController::addMesh(const std::shared_ptr<Mesh>& mesh)
{
    // TODO: Check mesh name conflict
    meshes.push_back(mesh);

    emit(onMeshListChanged(meshes));
}

void SceneController::deleteMesh(const std::shared_ptr<Mesh>& mesh)
{
    auto it = std::find(meshes.begin(), meshes.end(), mesh);

    if (it != meshes.end()) {
        meshes.erase(it);

        emit(onMeshListChanged(meshes));
    }
}
