#include "ContactResolver.h"

namespace P6 {
    ContactResolver::ContactResolver(unsigned iterations)
        : iterations(iterations), iterationsUsed(0) {}

    void ContactResolver::resolveContacts(std::vector<ParticleContact*>& contacts, float dt) {
        iterationsUsed = 0;

        while (iterationsUsed < iterations) {
            int bestIndex = -1;
            float bestSepSpeed = 0.0f;
            float bestPenetration = 0.0f;

            // 1. Find the contact with least separating speed OR penetration > 0
            for (size_t i = 0; i < contacts.size(); ++i) {
                float sepSpeed = contacts[i]->calculateSeparatingSpeed();
                float penetration = contacts[i]->penetration;

                if (penetration > 0 || sepSpeed < bestSepSpeed) {
                    bestSepSpeed = sepSpeed;
                    bestPenetration = penetration;
                    bestIndex = (int)i;
                }
            }

            if (bestIndex == -1) break;

            // 2. Resolve that contact
            contacts[bestIndex]->resolve(dt);
            iterationsUsed++;

            // 3. Stop if all contacts are separating and penetration resolved
            bool done = true;
            for (auto* c : contacts) {
                if (c->calculateSeparatingSpeed() < 0 || c->penetration > 0) {
                    done = false;
                    break;
                }
            }
            if (done) break;
        }
    }
}
