#include "PhysicsWorld.h"

namespace P6 {

	//Add a particle to the list
	void PhysicsWorld::AddParticle(P6Particle* toadd)
	{
		//Adds a particle to the end of the list
		particles.push_back(toadd);
	}

	void PhysicsWorld::Update(float time)
	{
		//Update the list first before calling
		//the updates for the particles (clears out destroyed ones)
		UpdateParticleList();

		//Create an iterator that points at the start of the list
		for (std::list<P6Particle*>::iterator p = particles.begin();
			//Continue looping until the end of the list
			p != particles.end();
			//Move to the next particle
			p++)
		{
			//Call the particle's update
			(*p)->update(time);
		}
	}

	void PhysicsWorld::UpdateParticleList()
	{
		//Removes ALL particles in the list that
		//return true to the function below
		particles.remove_if(
			//Checks ALL the particles in this list
			//If their isDestroyed flag is true
			[](P6Particle* p) {
				return p->IsDestroyed();
			}
		);
	}

}