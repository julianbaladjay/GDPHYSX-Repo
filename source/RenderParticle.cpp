#include "RenderParticle.h"
#include "OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

void RenderParticle::draw(OpenGLShader& shader) {
    if (physicsParticle->IsDestroyed()) return;

    RenderObject->setColor(color);
    RenderObject->setPosition(physicsParticle->position);

    shader.use();
    shader.setMat4("projection", glm::value_ptr(RenderObject->projection));
    shader.setMat4("transform", glm::value_ptr(RenderObject->getTransform()));
    shader.setVec3("color", color);

    RenderObject->draw(shader, color);
}

void RenderParticle::drawLine(const glm::vec3& a, const glm::vec3& b) {
    glDisable(GL_LIGHTING); // if you use lighting
    glColor3f(1.0f, 1.0f, 1.0f); // white line

    glBegin(GL_LINES);
    glVertex3f(a.x, a.y, a.z);
    glVertex3f(b.x, b.y, b.z);
    glEnd();
}

