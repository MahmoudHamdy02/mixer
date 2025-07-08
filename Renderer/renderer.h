#pragma once

#include <GL/gl.h>
#include <qopenglextrafunctions.h>

#include <vector>

#include "camera.h"
#include "meshgl.h"
#include "pmp/mat_vec.h"
#include "scenecontroller.h"
#include "shader.h"

class Renderer : private QOpenGLExtraFunctions
{
    SceneController* scene;

    Camera camera;
    pmp::mat4 projection;
    pmp::mat4 view;
    pmp::mat4 model;

    std::vector<MeshGL> meshGLs;
    Shader* shader;
    GLuint VAO, VBO;

    struct Grid {
        GLuint VAO, VBO;
        Shader* shader;
    };
    Grid grid;

    void setupGrid();
    void renderGrid();

public:
    Renderer(SceneController* scene);
    void initialize();
    void resize(int width, int height);
    void render();

    void moveCamera(float offsetX, float offsetY);
};
