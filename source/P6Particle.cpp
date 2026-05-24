#include "P6Particle.h"

namespace P6 {
    void P6Particle::updatePosition(float deltaTime)
    {
		this->position = this->position + (this->velocity * deltaTime) + ((1.0f / 2.0f) * (this->acceleration * deltaTime * deltaTime));
    }

    void P6Particle::updateVelocity(float deltaTime)
    {
        this->velocity = this->velocity + (this->acceleration * deltaTime);
    }

    void P6Particle::update(float time)
    {
        this->updatePosition(time);
		this->updateVelocity(time);
    }

    P6Particle::P6Particle()
    {
		this->position = glm::vec3(0, 0, 0);
		this->velocity = glm::vec3(0, 0, 0);
		this->acceleration = glm::vec3(0, 0, 0  );
    }
}


