#pragma once
#include "ForceGenerator.h"

namespace P6 {
    class ParticleSpring : public ForceGenerator {
    private:
        P6Particle* other;       // the other particle connected by the spring
        float springConstant;    // stiffness of the spring
        float restLength;        // rest length when no force is applied
    public:
        ParticleSpring(P6Particle* other, float k, float rest)
            : other(other), springConstant(k), restLength(rest) {}

        void updateForce(P6Particle* particle, float time) override;
    };
}
