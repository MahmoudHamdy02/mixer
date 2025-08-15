#include "scenecontroller.h"

#include <memory>
#include <vector>

#include "mesh.h"

SceneController::SceneController()
{
    meshes.push_back(std::make_unique<Mesh>("Sphere1"));
    meshes.push_back(std::make_unique<Mesh>("Sphere2"));
    meshes[1]->translate(pmp::vec3(0, 0, -3));
    emit(onMeshListChanged(meshes));
}

const std::vector<std::shared_ptr<Mesh>>& SceneController::getMeshes() const
{
    return meshes;
}

void SceneController::deleteMesh(const std::shared_ptr<Mesh>& mesh)
{
    auto it = std::find(meshes.begin(), meshes.end(), mesh);

    if (it != meshes.end()) {
        emit(onMeshDeleted(mesh));

        meshes.erase(it);

        emit(onMeshListChanged(meshes));
    }
}
