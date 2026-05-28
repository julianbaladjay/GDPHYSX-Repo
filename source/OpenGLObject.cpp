#include "OpenGLObject.h"
#include "tiny_obj_loader.h"

OpenGLObject::OpenGLObject(tinyobj::attrib_t& attrib, tinyobj::shape_t& shape) 
{
    for (auto& idx : shape.mesh.indices)
        indices.push_back(idx.vertex_index);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, attrib.vertices.size() * sizeof(float),
        attrib.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
        indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void OpenGLObject::setOrthographic(float left, float right, float bottom, float top, float near, float far) {
    projection = glm::ortho(left, right, bottom, top, near, far);
}

void OpenGLObject::setPerspective(float fov, float aspect, float near, float far) {
    projection = glm::perspective(glm::radians(fov), aspect, near, far);
}

glm::mat4 OpenGLObject::getTransform() const {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
    transform = glm::scale(transform, scale);
    if (theta != 0.0f) {
        transform = glm::rotate(transform, glm::radians(theta), glm::normalize(rotation));
    }
    return transform;
}

void OpenGLObject::draw() 
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void OpenGLObject::setPosition(const glm::vec3& pos) {
    position = pos;
}

void OpenGLObject::setColor(const glm::vec3& col) {
    // You’ll need a color field in OpenGLObject
    color = col;
}

void OpenGLObject::setDefaults() {
    position = glm::vec3(0.0f);
    scale = glm::vec3(100.0f);
    rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    theta = 0.0f;
    color = glm::vec3(1.0f, 0.0f, 0.0f); // default red
}

void OpenGLObject::cleanup() 
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

