#pragma once
#include "P6Particle.h"

namespace P6 {

	class PhysicsWorld
	{
	public:
		//holds all particles
		std::list<P6Particle> particles;

		//add particles
		void AddParticle(P6Particle* toadd);
		//world update
		void Update(float time);

	private:
		//update list of particles
		void UpdateParticleList();

	};

}



