#include "ForceGenerator.h"

namespace P6 {
    // Gravity
    GravityForceGenerator::GravityForceGenerator(const glm::vec3& g) : gravity(g) {}

    void GravityForceGenerator::updateForce(P6Particle* particle, float dt) {
        if (particle->mass <= 0.0f) return;
        particle->addForce(gravity * particle->mass);
    }

    // Drag
    DragForceGenerator::DragForceGenerator(float k1, float k2) : k1(k1), k2(k2) {}

    void DragForceGenerator::updateForce(P6Particle* particle, float dt) {
        glm::vec3 v = particle->velocity;
        float speed = glm::length(v);
        if (speed <= 0.0f) return;

        // Drag magnitude
        float dragMag = k1 * speed + k2 * speed * speed;

        // Drag force opposite to velocity
        glm::vec3 dragForce = -glm::normalize(v) * dragMag;
        particle->addForce(dragForce);
    }
}
