#pragma once
#include "ForceGenerator.h"

namespace P6 {
    class AnchoredSpring : public ForceGenerator {
    private:
        glm::vec3 anchor;
        float springConstant;
        float restLength;
    public:
        AnchoredSpring(const glm::vec3& anchor, float k, float rest)
            : anchor(anchor), springConstant(k), restLength(rest) {}

        void updateForce(P6Particle* particle, float time) override;
    };
}
