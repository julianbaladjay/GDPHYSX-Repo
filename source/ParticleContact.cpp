#include "ParticleContact.h"

namespace P6 {
    void ParticleContact::resolve(float dt) {
        resolveVelocity(dt);
        resolveInterpenetration(dt);
    }

    float ParticleContact::calculateSeparatingSpeed() const {
        glm::vec3 relativeVel = particles[0]->velocity;
        if (particles[1]) relativeVel -= particles[1]->velocity;
        return glm::dot(relativeVel, contactNormal);
    }

    void ParticleContact::resolveVelocity(float dt) {
        float separatingSpeed = calculateSeparatingSpeed();
        if (separatingSpeed > 0) return;

        float newSepSpeed = -separatingSpeed * restitution;
        float deltaSpeed = newSepSpeed - separatingSpeed;

        float invMass0 = 1.0f / particles[0]->mass;
        float invMass1 = particles[1] ? (1.0f / particles[1]->mass) : 0.0f;
        float totalInvMass = invMass0 + invMass1;
        if (totalInvMass <= 0) return;

        glm::vec3 impulse = (deltaSpeed / totalInvMass) * contactNormal;

        if (particles[1]) {
            particles[0]->velocity += impulse * invMass0;
            particles[1]->velocity -= impulse * invMass1;
        }
        else {
            // Anchor case: apply full correction to particle[0]
            particles[0]->velocity += impulse * invMass0;
        }
    }

    void ParticleContact::resolveInterpenetration(float dt) {
        if (penetration <= 0) return;

        float invMass0 = 1.0f / particles[0]->mass;
        float invMass1 = particles[1] ? (1.0f / particles[1]->mass) : 0.0f;
        float totalInvMass = invMass0 + invMass1;
        if (totalInvMass <= 0) return;

        glm::vec3 movePerInvMass = (penetration / totalInvMass) * contactNormal;

        if (particles[1]) {
            particles[0]->position += movePerInvMass * invMass0;
            particles[1]->position -= movePerInvMass * invMass1;
        }
        else {
            // Anchor case: snap particle back toward anchor
            particles[0]->position -= contactNormal * penetration;
        }

        penetration = 0;
    }

}
