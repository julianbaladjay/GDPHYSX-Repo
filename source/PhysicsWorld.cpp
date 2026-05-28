#include "PhysicsWorld.h"

namespace P6 {

	void PhysicsWorld::AddParticle(P6Particle* toadd)
	{
		particles.push_back(*toadd);
	}

	void PhysicsWorld::Update(float time)
	{
		//for (std::list<P6Particle>::iterator p = particles.begin(); p != particles.end(); ++p) 
		//{
			//(*p)->update(time);
		//}
	}

	void PhysicsWorld::UpdateParticleList()
	{
		// Implementation for updating the particle list
	}

}