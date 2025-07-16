#include "scenecontroller.h"

#include <vector>

#include "mesh.h"

SceneController::SceneController()
{
    meshes.emplace_back("Sphere1");
    meshes.emplace_back("Sphere2");
    meshes[1].translate(pmp::vec3(0, 0, -3));
}

std::vector<Mesh>& SceneController::getMeshes()
{
    return meshes;
}
