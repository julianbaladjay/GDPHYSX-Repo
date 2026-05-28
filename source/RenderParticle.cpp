#include "RenderParticle.h"

void RenderParticle::draw()
{
	//set the color of the render object to the color of the render particle
	RenderObject->setColor(color);
	//set the position of the render object to the position of the physics particle
	RenderObject->setPosition(physicsParticle->position);
	//draw the render object
	RenderObject->draw();
}
