#include "Cable.h"
#include <glm/glm.hpp>

namespace P6 {
    ParticleContact* Cable::getContact() const {
        if (!particles[0]) return nullptr;

        float length = currentLength();
        if (length <= maxLength) return nullptr;

        auto* contact = new ParticleContact();
        contact->particles[0] = particles[0];
        contact->particles[1] = nullptr; // anchor is immovable
        contact->penetration = length - maxLength;
        contact->contactNormal = glm::normalize(particles[0]->position - anchor);
        contact->restitution = restitution;
        return contact;
    }
}
