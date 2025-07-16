#pragma once

#include <GL/gl.h>
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
public:
    // TODO: Connect this to toolbar state
    enum class RenderMode { Flat, Wireframe, Rendered };
    enum class EditMode { Object, Vertex };

    Renderer(SceneController* scene);
    void initialize();
    void resize(int width, int height);
    void render();

    Ray mouseToWorldRay(float mouseX, float mouseY) const;
    void updateMesh(const std::string& name);

    const pmp::mat4& getModelMatrix() const;
    const pmp::mat4& getViewMatrix() const;
    const pmp::mat4& getProjectionMatrix() const;

    void setRenderMode(RenderMode mode);
    EditMode getEditMode();
    void setEditMode(EditMode mode);

    const Camera& getCamera() const;
    void moveCamera(float offsetX, float offsetY);
    void panCamera(float offsetX, float offsetY);
    void zoomCamera(float distance);

    void setSelectionRectangleVertices(const pmp::vec2& min, const pmp::vec2& max);

private:
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

    RenderMode renderMode = RenderMode::Flat;
    EditMode editMode = EditMode::Object;
};
