#pragma once

#include <qopenglextrafunctions.h>

#include <vector>

#include "camera.h"
#include "meshgl.h"
#include "pmp/mat_vec.h"
#include "scenecontroller.h"
#include "shader.h"

class Renderer : public QOpenGLExtraFunctions
{
    SceneController* scene;

    Camera camera;
    Shader* shader;
    pmp::mat4 projection;
    pmp::mat4 view;
    pmp::mat4 model;

    std::vector<MeshGL> meshGLs;
    GLuint VAO, VBO;
    GLuint shaderProgram;

    GLuint compileShader(const char* source, GLenum type);
    GLuint createShaderProgram();

public:
    Renderer(SceneController* scene);
    void initialize();
    void resize(int width, int height);
    void render();
};
