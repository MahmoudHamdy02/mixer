#pragma once

#include <qobject.h>
#include <qtmetamacros.h>

#include <vector>

#include "mesh.h"

class SceneController : public QObject
{
    Q_OBJECT

    std::vector<Mesh> meshes;

public:
    SceneController();
    std::vector<Mesh>& getMeshes();

signals:
    void onMeshListChanged(std::vector<Mesh>& meshes);
};
