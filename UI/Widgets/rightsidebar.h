#pragma once

#include <qboxlayout.h>
#include <qlistwidget.h>
#include <qtmetamacros.h>
#include <qwidget.h>

#include <vector>

#include "mesh.h"

class RightSidebar : public QWidget
{
    Q_OBJECT

    QListWidget* objectsList;

public:
    RightSidebar(QWidget* parent = nullptr);

public slots:
    void setMeshList(const std::vector<Mesh>& meshes);
};
