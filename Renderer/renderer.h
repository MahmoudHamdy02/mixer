#pragma once

#include <qopenglextrafunctions.h>

#include <vector>

#include "camera.h"
#include "grid.h"
#include "meshgl.h"
#include "pmp/mat_vec.h"
#include "ray.h"
#include "scenecontroller.h"
#include "selectionrectangle.h"
#include "shader.h"

class Renderer : private QOpenGLExtraFunctions
{
    SceneController* scene;

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

    std::unique_ptr<Grid> grid;
    std::unique_ptr<SelectionRectangle> selectionRectangle;

public:
    Renderer(SceneController* scene);
    void initialize();
    void resize(int width, int height);
    void render();

    Ray mouseToWorldRay(float mouseX, float mouseY) const;
    void updateMesh(const std::string& name);

    const pmp::mat4& getModelMatrix() const;
    const pmp::mat4& getViewMatrix() const;
    const pmp::mat4& getProjectionMatrix() const;

    const Camera& getCamera() const;
    void moveCamera(float offsetX, float offsetY);
    void panCamera(float offsetX, float offsetY);
    void zoomCamera(float distance);

    void setSelectionRectangleVertices(const pmp::vec2& min, const pmp::vec2& max);
};
