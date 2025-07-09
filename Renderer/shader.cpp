#include "shader.h"

#include <GL/gl.h>
#include <qfile.h>

#include "pmp/mat_vec.h"

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
    glProgramUniform1i(ID, glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value)
{
    glProgramUniform1i(ID, glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value)
{
    glProgramUniform1f(ID, glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, pmp::vec3 vec)
{
    glProgramUniform3f(ID, glGetUniformLocation(ID, name.c_str()), vec[0], vec[1], vec[2]);
}

void Shader::setVec3(const std::string &name, float v1, float v2, float v3)
{
    glProgramUniform3f(ID, glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}

void Shader::setMatrix4(const std::string &name, float *value)
{
    glProgramUniformMatrix4fv(ID, glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
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
