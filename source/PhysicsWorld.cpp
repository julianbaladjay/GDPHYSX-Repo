#include "PhysicsWorld.h"

namespace P6 {

	//add particle to list
	void PhysicsWorld::AddParticle(P6Particle* toadd)
	{
		//adds particle to end of list
		particles.push_back(toadd);
	}

	void PhysicsWorld::Update(float time)
	{
		//iterator that points at the start of the list
		for (std::list<P6Particle*>::iterator p = particles.begin(); 
			//continue looping until the end of list
			p != particles.end(); 
			//move to next particle
			++p) 
		{
			//call particle's update
			(*p)->update(time);
		}
	}

	void PhysicsWorld::UpdateParticleList()
	{
		// Implementation for updating the particle list
	}

}