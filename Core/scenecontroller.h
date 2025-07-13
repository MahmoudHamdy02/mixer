#pragma once

#include <vector>

#include "mesh.h"

class SceneController
{
    std::vector<Mesh> meshes;

public:
    SceneController();
    std::vector<Mesh>& getMeshes();
};
