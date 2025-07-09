#pragma once

#include <GL/gl.h>
#include <qopenglextrafunctions.h>

#include <string>

#include "pmp/mat_vec.h"

class Shader : protected QOpenGLExtraFunctions
{
public:
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath);
    void use();
    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setVec3(const std::string &name, pmp::vec3 vec);
    void setVec3(const std::string &name, float v1, float v2, float v3);
    void setMatrix4(const std::string &name, float *value);

private:
    GLuint compileShader(const char *source, GLenum type);
};
