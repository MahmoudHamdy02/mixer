#include "renderer.h"

#include <qfile.h>
#include <qlogging.h>

#include "mesh.h"
#include "meshgl.h"
#include "pmp/mat_vec.h"
#include "shader.h"

Renderer::Renderer(SceneController* scene) : scene(scene) {}

void Renderer::initialize()
{
    initializeOpenGLFunctions();

    // Setup shaders
    shader = new Shader(":/Renderer/Shaders/vertex.glsl", ":/Renderer/Shaders/fragment.glsl");
    shader->use();

    // Setup model and view matrices
    // Projection is set in resize() as it is called automatically on startup
    model = pmp::mat4::identity();
    shader->setMatrix4("model", model.data());
    view = pmp::translation_matrix(pmp::vec3(0.0f, 0.0f, -3.0f));
    shader->setMatrix4("view", view.data());

    // Triangle vertices
    float vertices[] = {
        0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f,
    };

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

    // shaderProgram = createShaderProgram();
}

void Renderer::resize(int width, int height)
{
    projection = pmp::perspective_matrix(45.0f, (float)width / height, 0.1f, 500.0f);
    shader->setMatrix4("projection", projection.data());
    glViewport(0, 0, width, height);
}

void Renderer::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // glUseProgram(shaderProgram);
    shader->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
