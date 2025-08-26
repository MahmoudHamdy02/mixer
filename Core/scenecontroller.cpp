#include "scenecontroller.h"

#include <memory>
#include <string>
#include <vector>

#include "mesh.h"

const std::vector<std::shared_ptr<Mesh>>& SceneController::getMeshes() const
{
    return meshes;
}

std::string SceneController::getUniqueMeshName(std::string name)
{
    // Add mesh name with value 1 to map if it doesn't exist
    if (nameCounter.find(name) == nameCounter.end()) {
        nameCounter[name] = 1;
        return name;
    }

    int count = nameCounter[name];
    std::string newName = name + "." + std::to_string(count);
    nameCounter[name] = ++count;
    return newName;
}

void SceneController::addMesh(const std::shared_ptr<Mesh>& mesh)
{
    auto it = std::find(meshes.begin(), meshes.end(), mesh);

    // Don't add if it already exists
    if (it != meshes.end())
        return;

    mesh->setName(getUniqueMeshName(mesh->getName()));
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
