#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace tinyobj {
    struct attrib_t;
    struct shape_t;
}

class OpenGLObject
{
public:
    GLuint VAO, VBO, EBO;
    std::vector<GLuint> indices;

    glm::vec3 position, scale, rotation;
    float theta;

    glm::mat4 projection;
    OpenGLObject(tinyobj::attrib_t& attrib, tinyobj::shape_t& shape);

    void setOrthographic(float left, float right, float bottom, float top, float near, float far);
    void setPerspective(float fov, float aspect, float near, float far);

    glm::mat4 getTransform() const;

    void draw();
    void cleanup();
};

