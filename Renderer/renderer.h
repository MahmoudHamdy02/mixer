#pragma once

#include <qobject.h>
#include <qopenglextrafunctions.h>
#include <qtmetamacros.h>

#include <memory>
#include <vector>

#include "camera.h"
#include "grid.h"
#include "meshgl.h"
#include "pmp/mat_vec.h"
#include "ray.h"
#include "scenecontroller.h"
#include "selectionmanager.h"
#include "selectionrectangle.h"
#include "shader.h"

class Renderer : public QObject, private QOpenGLExtraFunctions
{
    Q_OBJECT

    SceneController* scene;
    SelectionManager* selectionManager;

    std::vector<std::shared_ptr<MeshGL>> meshGLs;

    Camera camera;
    pmp::mat4 projection;
    pmp::mat4 view;
    pmp::mat4 model;
    int screenWidth;
    int screenHeight;

    // Main rendering modes
    std::unique_ptr<Shader> flatShader;
    std::unique_ptr<Shader> wireframeShader;
    std::unique_ptr<Shader> renderedShader;

    std::unique_ptr<Shader> pointsShader;
    std::unique_ptr<Shader> outlineShader;

    std::unique_ptr<Grid> grid;

    bool drawSelectionRectangle = false;
    std::unique_ptr<SelectionRectangle> selectionRectangle;

    void drawMesh(const std::shared_ptr<MeshGL>& meshGL, bool outlined);

public:
    Renderer(SceneController* scene, SelectionManager* selectionManager);
    void initialize();
    void resize(int width, int height);
    void render();

    const std::shared_ptr<MeshGL> getMeshGLFromMesh(const std::shared_ptr<Mesh>& mesh);
    float getPixelDepth(int screenX, int screenY);
    void setDrawSelectionRectangle(bool value);

    Ray mouseToWorldRay(float mouseX, float mouseY) const;
    void updateMesh(const std::string& name);
    void updateMeshes();

    const pmp::mat4& getModelMatrix() const;
    const pmp::mat4& getViewMatrix() const;
    const pmp::mat4& getProjectionMatrix() const;
    const pmp::mat4 getMVPMatrix() const;

    const Camera& getCamera() const;
    void moveCamera(float offsetX, float offsetY);
    void panCamera(float offsetX, float offsetY);
    void zoomCamera(float distance);

    void setSelectionRectangleVertices(const pmp::vec2& min, const pmp::vec2& max);

    void addMeshGL(const std::shared_ptr<MeshGL>& meshGL);
    void deleteMeshGL(const std::shared_ptr<MeshGL>& MeshGL);
};
