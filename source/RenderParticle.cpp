#include "RenderParticle.h"
#include "OpenGLShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void RenderParticle::draw(OpenGLShader& shader)
{
    //set the color of the render object to the color of the render particle
    RenderObject->setColor(color);
    //set the position of the render object to the position of the physics particle
    RenderObject->setPosition(physicsParticle->position);
    
    //shader usage
    shader.use();
    shader.setMat4("projection", glm::value_ptr(RenderObject->projection));
    shader.setMat4("transform", glm::value_ptr(RenderObject->getTransform()));
    shader.setVec3("color", color);

    //draw the render object
    RenderObject->draw(shader, color);
}
