#include "scenecontroller.h"

#include <vector>

#include "mesh.h"

SceneController::SceneController()
{
    meshes.emplace_back();
}

std::vector<Mesh>& SceneController::getMeshes()
{
    return meshes;
}
