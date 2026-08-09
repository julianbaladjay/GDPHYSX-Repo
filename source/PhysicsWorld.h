#pragma once
#include <list>
#include "P6Particle.h"

namespace P6 {

	class PhysicsWorld
	{
	public:
		//holds pointers to ALL our particles
		//(pointers, not copies, so this stays in sync with whatever
		//owns the actual particle, e.g. a RenderParticle in main.cpp)
		std::list<P6Particle*> particles;

		//add particles
		void AddParticle(P6Particle* toadd);
		//world update
		void Update(float time);

	private:
		//update list of particles
		void UpdateParticleList();

	};

}



