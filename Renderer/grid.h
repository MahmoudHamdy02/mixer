#pragma once

#include <qopenglextrafunctions.h>
#include <memory>

#include "shader.h"

class Grid : private QOpenGLExtraFunctions
{
    GLuint VAO, VBO;

public:
    std::unique_ptr<Shader> shader;

    Grid();
    void render();
};
