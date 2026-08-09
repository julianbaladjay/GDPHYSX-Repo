#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace P6 {
	class P6Particle
	{
		protected:
			void updatePosition(float deltaTime);
			void updateVelocity(float deltaTime);

			//Flag to hold when to destroy the Particle
			bool isDestroyed = false;
		public:
			glm::vec3 position;
			glm::vec3 velocity;
			glm::vec3 acceleration;

			void update(float deltaTime);

			//Will set our flag
			void Destroy();
			//Getter for the isDestroyed variable
			bool IsDestroyed() const { return isDestroyed; }

			P6Particle();
	};
} // namespace P6
