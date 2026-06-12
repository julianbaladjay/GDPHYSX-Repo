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
			//flag to hold when to destroy particle
			bool isDestroyed = false;
		public:
			glm::vec3 position;
			glm::vec3 velocity;
			glm::vec3 acceleration;

			void update(float deltaTime);

			void Destroy();

			bool IsDestroyed() { 
				return isDestroyed; 
			}

			P6Particle();
	};
} // namespace P6
