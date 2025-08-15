#pragma once

#include <memory>
#include <unordered_set>

#include "mesh.h"
#include "pmp/mat_vec.h"
#include "ray.h"
#include "scenecontroller.h"

class SelectionManager
{
    SceneController* scene;

    std::unordered_set<std::weak_ptr<Mesh>> selectedMeshes;

public:
    SelectionManager(SceneController* scene);

    const std::unordered_set<std::weak_ptr<Mesh>>& getSelectedMeshes() const;

    bool isMeshSelected(const std::shared_ptr<Mesh>& mesh);

    void selectVerticesInRectangle(const pmp::vec2& min, const pmp::vec2& max, const pmp::mat4& mvp,
                                   const pmp::vec3& cameraDir) const;
    void selectVertex(float ndcX, float ndcY, float depthBufferValue, const pmp::mat4& mvp,
                      const pmp::vec3& cameraDir) const;
    void selectObjectsInRectangle(const pmp::vec2& clipMin, const pmp::vec2& clipMax, const pmp::mat4& view,
                                  const pmp::mat4& projection);
    void selectObject(const Ray& ray);

    void resetSelectedObjects();
};
