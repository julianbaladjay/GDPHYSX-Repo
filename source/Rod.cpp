#include "Rod.h"
#include <glm/glm.hpp>

namespace P6 {
    Rod::Rod(float length) : rodLength(length) {}

    ParticleContact* Rod::getContact() const {
        float length = currentLength();

        if (length == rodLength) return nullptr; // no contact needed

        ParticleContact* contact = new ParticleContact();
        contact->particles[0] = particles[0];
        contact->particles[1] = particles[1];

        // Calculate penetration
        if (length > rodLength) {
            contact->penetration = length - rodLength;
            contact->contactNormal = glm::normalize(particles[0]->position - particles[1]->position);
        }
        else {
            contact->penetration = rodLength - length;
            contact->contactNormal = glm::normalize(particles[1]->position - particles[0]->position);
        }

        contact->restitution = 0.0f; // rods don’t bounce
        return contact;
    }
}
