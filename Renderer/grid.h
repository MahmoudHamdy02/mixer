#pragma once

#include <qopenglextrafunctions.h>

#include "shader.h"

class Grid : private QOpenGLExtraFunctions
{
    GLuint VAO, VBO;

public:
    Shader* shader;

    Grid();
    void render();
};
