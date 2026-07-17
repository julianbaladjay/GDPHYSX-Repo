#include "ParticleSpring.h"
#include <glm/glm.hpp>

namespace P6 {
    void ParticleSpring::updateForce(P6Particle* particle, float time) {
        // Get the current position of the particle
        glm::vec3 pos = particle->position;

        // Create a vector from this particle to the other particle
        glm::vec3 force = pos - other->position;

        // Get the magnitude of the vector above
        float mag = glm::length(force);

        // Hooke's Law: f = -k * |l_curr - l_rest|
        float springForce = -springConstant * fabs(mag - restLength);

        // Get the direction from the particle to the other particle
        force = glm::normalize(force);

        // Apply the force using the direction above
        force *= springForce;

        // Add Force!
        particle->addForce(force);
    }
}
