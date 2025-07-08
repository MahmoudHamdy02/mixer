#include "shader.h"

#include <GL/gl.h>
#include <qfile.h>

// constructor generates the shader on the fly
Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    initializeOpenGLFunctions();
    // Read shaders from file
    QFile vfile(vertexPath);
    QFile ffile(fragmentPath);
    vfile.open(QIODevice::ReadOnly | QIODevice::Text);
    ffile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray vsrc = vfile.readAll();
    QByteArray fsrc = ffile.readAll();

    GLuint vertexShader = compileShader(vsrc.constData(), GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fsrc.constData(), GL_FRAGMENT_SHADER);

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    GLint success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(ID, 512, nullptr, log);
        qCritical() << "Program linking failed:" << log;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// activate the shader
void Shader::use()
{
    glUseProgram(ID);
}

// utility uniform functions
void Shader::setBool(const std::string &name, bool value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value)
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, float v1, float v2, float v3)
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}

void Shader::setMatrix4(const std::string &name, float *value)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_TRUE, value);
}

GLuint Shader::compileShader(const char *source, GLenum type)
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
