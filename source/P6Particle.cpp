#include "P6Particle.h"

namespace P6 {
    void P6Particle::addForce(const glm::vec3& f) {
        forceAccum += f;
    }

    void P6Particle::clearForces() {
        forceAccum = glm::vec3(0);
    }

    void P6Particle::integrate(float dt) {
        if (mass <= 0.0f) return;

        // Acceleration from accumulated forces
        acceleration = forceAccum / mass;

        // Update velocity
        velocity += acceleration * dt;

        // Apply damping (drag)
        velocity *= pow(damping, dt);

        // Update position
        position += velocity * dt;

        // Clear forces for next frame
        clearForces();
    }

    void P6Particle::Destroy() {
        isDestroyed = true;
    }

    P6Particle::P6Particle() {
        // Initialize defaults (can be overridden later)
        position = glm::vec3(0.0f);
        velocity = glm::vec3(0.0f);
        acceleration = glm::vec3(0.0f);
        forceAccum = glm::vec3(0.0f);

        mass = 50.0f;
        damping = 0.99f;
        radius = 10.0f;
        restitution = 0.9f;
    }
}


