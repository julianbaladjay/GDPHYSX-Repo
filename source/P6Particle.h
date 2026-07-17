#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace P6 {
    class P6Particle {
    public:
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 acceleration;
        glm::vec3 forceAccum;

        float mass;
        float damping;
        float radius;
        float restitution;
        bool isDestroyed = false;

        void addForce(const glm::vec3& f);
        void clearForces();
        void integrate(float dt);

        void Destroy();
        bool IsDestroyed() { return isDestroyed; }

        P6Particle();
    };
}