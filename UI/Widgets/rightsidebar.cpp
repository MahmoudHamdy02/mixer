#include "rightsidebar.h"

#include <qboxlayout.h>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistwidget.h>
#include <qsplitter.h>
#include <qwidget.h>

#include <memory>

RightSidebar::RightSidebar(QWidget* parent) : QWidget(parent)
{
    QSplitter* verticalLayout = new QSplitter(Qt::Orientation::Vertical, this);
    verticalLayout->setChildrenCollapsible(false);

    // Scene objects label
    QLabel* sceneLabel = new QLabel("Scene objects");
    verticalLayout->addWidget(sceneLabel);

    // Scene objects list
    objectsList = new QListWidget(this);
    verticalLayout->addWidget(objectsList);

    // Object properties label
    QLabel* propertiesLabel = new QLabel("Properties");
    verticalLayout->addWidget(propertiesLabel);

    // Object properties grid
    QWidget* propertiesGrid = new QWidget(this);
    QGridLayout* propertiesGridLayout = new QGridLayout(this);
    propertiesGrid->setLayout(propertiesGridLayout);
    verticalLayout->addWidget(propertiesGrid);

    // Grid items
    QLabel* xLabel = new QLabel("X");
    propertiesGridLayout->addWidget(xLabel, 0, 0);
    QLabel* yLabel = new QLabel("Y");
    propertiesGridLayout->addWidget(yLabel, 1, 0);
    QLabel* zLabel = new QLabel("Z");
    propertiesGridLayout->addWidget(zLabel, 2, 0);

    QLineEdit* xPos = new QLineEdit("0.0");
    propertiesGridLayout->addWidget(xPos, 0, 1);
    QLineEdit* yPos = new QLineEdit("0.0");
    propertiesGridLayout->addWidget(yPos, 1, 1);
    QLineEdit* zPos = new QLineEdit("0.0");
    propertiesGridLayout->addWidget(zPos, 2, 1);
}

void RightSidebar::setMeshList(const std::vector<std::shared_ptr<Mesh>>& meshes)
{
    objectsList->clear();
    for (const std::shared_ptr<Mesh>& mesh : meshes) {
        const std::string& name = mesh->getName();
        new QListWidgetItem(QString::fromStdString(name), objectsList);
    }
}
