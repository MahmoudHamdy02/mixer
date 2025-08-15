#include "scenecontroller.h"

#include <memory>
#include <vector>

#include "mesh.h"

SceneController::SceneController()
{
    meshes.push_back(std::make_unique<Mesh>("Sphere1"));
    meshes[0]->translate(pmp::vec3(3, 0, 0));
    meshes.push_back(std::make_unique<Mesh>("Sphere2"));
    meshes[1]->translate(pmp::vec3(0, 0, -3));
    meshes.push_back(std::make_unique<Mesh>("Sphere3"));
    meshes[2]->translate(pmp::vec3(0, 0, -9));
    emit(onMeshListChanged(meshes));
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
