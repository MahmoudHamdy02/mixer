#include "scenecontroller.h"

#include <vector>

#include "mesh.h"

SceneController::SceneController()
{
    meshes.emplace_back("Sphere1");
    meshes.emplace_back("Sphere2");
    meshes[1].translate(pmp::vec3(0, 0, -3));
    emit(onMeshListChanged(meshes));
}

std::vector<Mesh>& SceneController::getMeshes()
{
    return meshes;
}

void SceneController::deleteMesh(Mesh* mesh)
{
    auto it = std::find_if(meshes.begin(), meshes.end(), [&](Mesh& elem) { return &elem == mesh; });

    if (it != meshes.end()) {
        emit(onMeshDeleted(mesh));
        emit(onMeshListChanged(meshes));

        meshes.erase(it);
    }
}
