#pragma once
#include <string>
#include "P6Particle.h"
#include "OpenGLObject.h"

class OpenGLShader;

class RenderParticle {
public:
    P6::P6Particle* physicsParticle;
    OpenGLObject* RenderObject;
    glm::vec3 color;
    std::string name;

    RenderParticle(P6::P6Particle* p, OpenGLObject* obj)
        : physicsParticle(p), RenderObject(obj) {
        color = glm::vec3(
            (rand() % 256) / 255.0f,
            (rand() % 256) / 255.0f,
            (rand() % 256) / 255.0f
        );
        RenderObject->scale = glm::vec3(p->radius);
    }

    void draw(OpenGLShader& shader);
    static void drawLine(const glm::vec3& a, const glm::vec3& b);
};
