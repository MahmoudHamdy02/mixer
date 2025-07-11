#pragma once

#include <GL/gl.h>
#include <qopenglextrafunctions.h>

#include <vector>

#include "camera.h"
#include "grid.h"
#include "meshgl.h"
#include "pmp/mat_vec.h"
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

    std::vector<MeshGL> meshGLs;
    Shader* flatShader;
    Shader* wireframeShader;
    Shader* pointsShader;

    std::unique_ptr<Grid> grid;
    std::unique_ptr<SelectionRectangle> selectionRectangle;

public:
    enum class RenderMode { Flat, Wireframe, Rendered };

    Renderer(SceneController* scene);
    void initialize();
    void resize(int width, int height);
    void render();

    void setRenderMode(RenderMode mode);

    void moveCamera(float offsetX, float offsetY);
    void panCamera(float offsetX, float offsetY);
    void zoomCamera(float distance);

    void setSelectionRectangleVertices(const pmp::vec2& min, const pmp::vec2& max);
    const void selectInsideRectangle(const pmp::vec2& min, const pmp::vec2& max) const;

private:
    RenderMode renderMode = RenderMode::Flat;
};
