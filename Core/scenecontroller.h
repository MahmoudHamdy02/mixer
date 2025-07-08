#pragma once

#include <vector>

#include "mesh.h"

class SceneController
{
    std::vector<Mesh> meshes;

public:
    SceneController();
    const std::vector<Mesh>& getMeshes() const;
};
