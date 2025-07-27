#pragma once

#include <unordered_set>

#include "mesh.h"
#include "pmp/mat_vec.h"
#include "ray.h"
#include "scenecontroller.h"

class SelectionManager
{
public:
    SelectionManager(SceneController* scene);

    bool isMeshSelected(Mesh*);

    void selectVerticesInRectangle(const pmp::vec2& min, const pmp::vec2& max, const pmp::mat4& mvp,
                                   const pmp::vec3& cameraDir) const;
    void selectVertex(float ndcX, float ndcY, float depthBufferValue, const pmp::mat4& mvp,
                      const pmp::vec3& cameraDir) const;
    void selectObjectsInRectangle(const pmp::vec2& min, const pmp::vec2& max);
    void selectObject(const Ray& ray);

private:
    SceneController* scene;

    std::unordered_set<Mesh*> selectedMeshes;
};
