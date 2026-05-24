#pragma once
#include <string>
#include <glad/glad.h>

class OpenGLShader
{
public:
    GLuint ID;
    OpenGLShader(const char* vertexPath, const char* fragmentPath);
    void use();
    void setMat4(const std::string& name, const float* value);
};

