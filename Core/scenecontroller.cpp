#include "scenecontroller.h"

#include <vector>

#include "mesh.h"

SceneController::SceneController()
{
    meshes.emplace_back(new Mesh("Sphere1"));
    meshes.emplace_back(new Mesh("Sphere2"));
    meshes[1]->translate(pmp::vec3(0, 0, -3));
    emit(onMeshListChanged(meshes));
}

const std::vector<Mesh*>& SceneController::getMeshes() const
{
    return meshes;
}

void SceneController::deleteMesh(Mesh* mesh)
{
    auto it = std::find(meshes.begin(), meshes.end(), mesh);

    if (it != meshes.end()) {
        emit(onMeshDeleted(mesh));

        delete *it;
        meshes.erase(it);

        emit(onMeshListChanged(meshes));
    }
}
