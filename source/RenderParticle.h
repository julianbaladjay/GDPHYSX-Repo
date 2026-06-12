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
	float radius;
	std::string name;

	RenderParticle(P6::P6Particle* p, OpenGLObject* obj) : physicsParticle(p), RenderObject(obj) 
	{
		//color = glm::vec3(1.0f, 0.0f, 0.0f); // Set default color to red
		// Random color
        color = glm::vec3(
            (rand() % 256) / 255.0f,
            (rand() % 256) / 255.0f,
            (rand() % 256) / 255.0f
        );

        // Random radius [2–10]
        radius = 2.0f + (rand() % 9);

        // Apply radius to render object
        RenderObject->scale = glm::vec3(radius);
	}

	//constructor in case we want to specify color
	RenderParticle(P6::P6Particle* p, OpenGLObject* obj, glm::vec3 c, const std::string& n)
		: physicsParticle(p), RenderObject(obj), color(c), name(n) {}

	void draw(OpenGLShader& shader);
};

