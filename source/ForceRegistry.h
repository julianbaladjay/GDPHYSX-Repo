#pragma once
#include <list>
#include "P6Particle.h"
#include "ForceGenerator.h"

namespace P6 {
    struct ForceRegistration {
        P6Particle* particle;
        ForceGenerator* generator;
    };

    class ForceRegistry {
    private:
        std::list<ForceRegistration> registrations;
    public:
        void add(P6Particle* p, ForceGenerator* g);
        void remove(P6Particle* p, ForceGenerator* g);
        void clear();
        void updateForces(float dt);
    };
}
