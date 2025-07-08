#include "renderer.h"

#include <qfile.h>

#include <iostream>

#include "mesh.h"
#include "meshgl.h"
#include "pmp/mat_vec.h"
#include "shader.h"

Renderer::Renderer(SceneController* scene) : scene(scene) {}

void Renderer::initialize()
{
    initializeOpenGLFunctions();
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

    shaderProgram = createShaderProgram();
}

void Renderer::resize(int width, int height)
{
    glViewport(0, 0, width, height);
}

void Renderer::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

GLuint Renderer::compileShader(const char* source, GLenum type)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        qCritical() << "Shader compilation failed:" << log;
    }
    return shader;
}

GLuint Renderer::createShaderProgram()
{
    // Read shaders from file
    QFile vfile("/home/mahmoud/Work/Projects/CG/mixer/Renderer/Shaders/vertex.glsl");
    QFile ffile("/home/mahmoud/Work/Projects/CG/mixer/Renderer/Shaders/fragment.glsl");
    vfile.open(QIODevice::ReadOnly | QIODevice::Text);
    ffile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray vsrc = vfile.readAll();
    QByteArray fsrc = ffile.readAll();

    GLuint vertexShader = compileShader(vsrc.constData(), GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fsrc.constData(), GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        qCritical() << "Program linking failed:" << log;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}
