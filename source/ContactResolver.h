#pragma once
#include <vector>
#include "ParticleContact.h"

namespace P6 {
    class ContactResolver {
    private:
        unsigned iterations;
        unsigned iterationsUsed;
    public:
        ContactResolver(unsigned iterations);
        void resolveContacts(std::vector<ParticleContact*>& contacts, float dt);
    };
}
