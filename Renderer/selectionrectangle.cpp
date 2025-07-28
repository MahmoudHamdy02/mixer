#include "selectionrectangle.h"

#include <memory>

#include "shader.h"

SelectionRectangle::SelectionRectangle()
{
    initializeOpenGLFunctions();

    // Defined in counter-clockwise order starting from bottom left
    // TODO: Don't render when not actively selecting
    float vertices[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    unsigned int indices[] = {0, 1, 3, 1, 2, 3};

    // Vertex Array Object + Vertex Buffer Object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // --- Element Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shader = std::make_unique<Shader>(":/selectionRectangleVertex.glsl", ":/selectionRectangleFragment.glsl");
}

void SelectionRectangle::setVertices(const pmp::vec2& min, const pmp::vec2& max)
{
    float vertices[] = {min[0], min[1], 0.0f, max[0], min[1], 0.0f, max[0], max[1], 0.0f, min[0], max[1], 0.0f};
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void SelectionRectangle::render()
{
    glBindVertexArray(VAO);
    shader->use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
