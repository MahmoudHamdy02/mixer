#pragma once

#include <qobject.h>
#include <qtmetamacros.h>

#include <memory>
#include <vector>

#include "mesh.h"

class SceneController : public QObject
{
    Q_OBJECT

    std::vector<std::shared_ptr<Mesh>> meshes;

public:
    SceneController();

    const std::vector<std::shared_ptr<Mesh>>& getMeshes() const;

    void addMesh(const std::shared_ptr<Mesh>& mesh);
    void deleteMesh(const std::shared_ptr<Mesh>& mesh);

signals:
    void onMeshListChanged(const std::vector<std::shared_ptr<Mesh>>& meshes);
};
