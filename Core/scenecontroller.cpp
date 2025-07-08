#include "scenecontroller.h"

#include <vector>

#include "mesh.h"

SceneController::SceneController()
{
    meshes.emplace_back();
}

const std::vector<Mesh>& SceneController::getMeshes() const
{
    return meshes;
}
