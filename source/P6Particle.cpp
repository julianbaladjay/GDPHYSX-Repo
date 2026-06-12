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
        // Apply gravity (constant downward acceleration)
        this->acceleration += glm::vec3(0.0f, -10.0f, 0.0f);

        this->updatePosition(time);
		this->updateVelocity(time);

        // Deduct lifespan
        this->lifespan -= time;

        // Destroy if lifespan expired
        if (lifespan <= 0.0f) {
            Destroy();
        }
    }

    P6Particle::P6Particle()
    {
        float angle = glm::radians((float)(rand() % 360));
        float radius = (rand() % 200) / 100.0f; // 0–2 m spread
        this->position = glm::vec3(
            radius * cos(angle) * 100.0f,  // X spread
            -380.0f,                       // bottom Y
            radius * sin(angle) * 100.0f   // Z spread
        );

        // Upward velocity with cone spread
        float coneAngle = glm::radians(10.0f); 
        float speed = 40.0f + (rand() % 40);  
        float vx = speed * sin(coneAngle) * cos(angle);
        float vy = speed * cos(coneAngle);    
        float vz = speed * sin(coneAngle) * sin(angle);
        this->velocity = glm::vec3(vx, vy, vz);

        this->acceleration = glm::vec3(0.0f, -10.0f, 0.0f); // gravity
        this->lifespan = 5.0f + (rand() % 6);               // 5–10 s
    }

	void P6Particle::Destroy()
	{
		this->isDestroyed = true;
	}

}


