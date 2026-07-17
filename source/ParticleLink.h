#pragma once
#include "P6Particle.h"
#include "ParticleContact.h"

namespace P6 {
    class ParticleLink {
    public:
        P6Particle* particles[2];

        virtual float currentLength() const;
        virtual ParticleContact* getContact() const = 0;
    };
}
