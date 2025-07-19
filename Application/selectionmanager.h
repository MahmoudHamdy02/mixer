#pragma once

#include <vector>

#include "mesh.h"
#include "pmp/mat_vec.h"
#include "ray.h"
#include "renderer.h"
#include "scenecontroller.h"

class SelectionManager
{
public:
    SelectionManager(Renderer* renderer, SceneController* scene);

    void selectVerticesInRectangle(const pmp::vec2& min, const pmp::vec2& max) const;
    void selectVertex(float ndcX, float ndcY, float depthBufferValue) const;
    void selectObjectsInRectangle(const pmp::vec2& min, const pmp::vec2& max) const;
    void selectObject(const Ray& ray) const;

private:
    Renderer* renderer;
    SceneController* scene;

    std::vector<Mesh*> selectedMeshes;
};
