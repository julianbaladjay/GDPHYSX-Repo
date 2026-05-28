#pragma once
#include <string>
#include "P6Particle.h"
#include "OpenGLObject.h"

class OpenGLShader;

class RenderParticle
{
public:
	P6::P6Particle* physicsParticle;
	OpenGLObject* RenderObject;
	glm::vec3 color;
	std::string name;
	bool finished = false;

	RenderParticle(P6::P6Particle* p, OpenGLObject* obj) : physicsParticle(p), RenderObject(obj) 
	{
		color = glm::vec3(1.0f, 0.0f, 0.0f); // Set default color to red
	}

	//constructor in case we want to specify color
	RenderParticle(P6::P6Particle* p, OpenGLObject* obj, glm::vec3 c, const std::string& n)
		: physicsParticle(p), RenderObject(obj), color(c), name(n) {}

	void draw(OpenGLShader& shader);
};

