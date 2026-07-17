#pragma once
#include "P6Particle.h"

namespace P6 {
    class ForceGenerator {
    public:
        virtual void updateForce(P6Particle* particle, float dt) = 0;
    };

    // Gravity
    class GravityForceGenerator : public ForceGenerator {
    private:
        glm::vec3 gravity;
    public:
        GravityForceGenerator(const glm::vec3& g);
        void updateForce(P6Particle* particle, float dt) override;
    };

    // Drag
    class DragForceGenerator : public ForceGenerator {
    private:
        float k1; // velocity coefficient
        float k2; // velocity^2 coefficient
    public:
        DragForceGenerator(float k1, float k2);
        void updateForce(P6Particle* particle, float dt) override;
    };
}
