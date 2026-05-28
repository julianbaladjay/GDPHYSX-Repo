#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class OpenGLShader
{
public:
    GLuint ID;
    OpenGLShader(const char* vertexPath, const char* fragmentPath);
    void use();
    void setMat4(const std::string& name, const float* value);
    void setVec3(const std::string& name, const float* value) const;

    // Overload for glm::vec3
    void setVec3(const std::string& name, const glm::vec3& value) const;
};

