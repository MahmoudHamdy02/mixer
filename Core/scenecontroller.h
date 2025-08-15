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

    void deleteMesh(const std::shared_ptr<Mesh>& mesh);

signals:
    void onMeshDeleted(const std::shared_ptr<Mesh>& mesh);
    void onMeshListChanged(std::vector<std::shared_ptr<Mesh>>& meshes);
};
