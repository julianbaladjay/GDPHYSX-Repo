#pragma once
#include "ParticleLink.h"
#include "P6Particle.h"
#include "ParticleContact.h"
#include <glm/glm.hpp>

namespace P6 {
    class Cable : public ParticleLink {
    public:
        glm::vec3 anchor;
        float maxLength;
        float restitution;

        Cable(const glm::vec3& anchorPoint, float length, float rest = 0.0f)
            : anchor(anchorPoint), maxLength(length), restitution(rest) {
            particles[0] = nullptr;
            particles[1] = nullptr;
        }

        // Override currentLength to use anchor instead of particles[1]
        float currentLength() const override {
            if (!particles[0]) return 0.0f;
            glm::vec3 diff = particles[0]->position - anchor;
            return glm::length(diff);
        }

        ParticleContact* getContact() const override;
    };
}
