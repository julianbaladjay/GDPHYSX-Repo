#pragma once
#include "P6Particle.h"
#include <glm/glm.hpp>

namespace P6 {
    class ParticleContact {
    public:
        P6Particle* particles[2];
        float restitution;
        glm::vec3 contactNormal;
        float penetration; // depth of interpenetration

        void resolve(float dt);

        // make separating speed public so ContactResolver can access it
        float calculateSeparatingSpeed() const;

    private:
        void resolveVelocity(float dt);
        void resolveInterpenetration(float dt);
    };
}
