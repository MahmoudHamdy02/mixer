#pragma once

#include <qopenglextrafunctions.h>

#include "pmp/mat_vec.h"
#include "shader.h"

class SelectionRectangle : private QOpenGLExtraFunctions
{
    GLuint VAO, VBO, EBO;

public:
    std::unique_ptr<Shader> shader;

    SelectionRectangle();

    void setVertices(const pmp::vec2& min, const pmp::vec2& max);
    void render();
};
