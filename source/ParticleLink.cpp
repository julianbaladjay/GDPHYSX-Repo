#include "ParticleLink.h"
#include <glm/glm.hpp>

namespace P6 {
    float ParticleLink::currentLength() const {
        glm::vec3 diff = particles[0]->position - particles[1]->position;
        return glm::length(diff);
    }
}
