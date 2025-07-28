#pragma once

#include <qopenglextrafunctions.h>

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

class Renderer : private QOpenGLExtraFunctions
{
    SceneController* scene;
    SelectionManager* selectionManager;

    Camera camera;
    pmp::mat4 projection;
    pmp::mat4 view;
    pmp::mat4 model;
    int screenWidth;
    int screenHeight;

    std::vector<MeshGL> meshGLs;
    Shader* flatShader;
    Shader* wireframeShader;
    Shader* pointsShader;
    Shader* outlineShader;

    std::unique_ptr<Grid> grid;
    std::unique_ptr<SelectionRectangle> selectionRectangle;

    void drawMesh(MeshGL& meshGL, bool outlined);

public:
    Renderer(SceneController* scene, SelectionManager* selectionManager);
    void initialize();
    void resize(int width, int height);
    void render();

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
};
