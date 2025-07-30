#pragma once

#include <qobject.h>
#include <qtmetamacros.h>

#include <vector>

#include "mesh.h"

class SceneController : public QObject
{
    Q_OBJECT

    std::vector<Mesh*> meshes;

public:
    SceneController();

    const std::vector<Mesh*>& getMeshes() const;

    void deleteMesh(Mesh* mesh);

signals:
    void onMeshDeleted(Mesh* mesh);
    void onMeshListChanged(std::vector<Mesh*>& meshes);
};
