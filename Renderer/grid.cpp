#include "grid.h"

Grid::Grid()
{
    initializeOpenGLFunctions();

    float vertices[] = {1.0f,  1.0f,  0.0f, -1.0f, -1.0f, 0.0f, -1.0f, 1.0f,  0.0f,
                        -1.0f, -1.0f, 0.0f, 1.0f,  1.0f,  0.0f, 1.0f,  -1.0f, 0.0f};

    // Vertex Array Object + Vertex Buffer Object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    shader = new Shader(":/Renderer/Shaders/gridVertex.glsl", ":/Renderer/Shaders/gridFragment.glsl");
}

void Grid::render()
{
    glBindVertexArray(VAO);
    shader->use();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
