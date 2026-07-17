#include "PhysicsWorld.h"
#include <iostream>
#include <glm/glm.hpp>

namespace P6 {

    PhysicsWorld::PhysicsWorld(unsigned maxIterations)
        : resolver(maxIterations) {}

    void PhysicsWorld::AddParticle(P6Particle* toadd) {
        particles.push_back(toadd);

        // Example: automatically register gravity
        static GravityForceGenerator gravity(glm::vec3(0.0f, -9.8f, 0.0f));
        registry.add(toadd, &gravity);
    }

    void PhysicsWorld::AddLink(ParticleLink* link) {
        links.push_back(link);
    }

    void PhysicsWorld::AddContact(ParticleContact* contact) {
        contacts.push_back(contact);
    }

    void PhysicsWorld::Update(float time) {
        // 1. Apply forces
        registry.updateForces(time);

        // 2. Integrate particles
        UpdateParticleList();
        for (auto* p : particles) {
            p->integrate(time);
        }

        // 3. Generate contacts (collisions + links)
        generateContacts();

        // 4. Resolve contacts
        resolveContacts(time);
    }

    void PhysicsWorld::UpdateParticleList() {
        particles.remove_if([](P6Particle* p) {
            return p->IsDestroyed();
            });
    }

    void PhysicsWorld::generateContacts() {
        contacts.clear();

        // Collision detection between particles
        for (auto itA = particles.begin(); itA != particles.end(); ++itA) {
            for (auto itB = std::next(itA); itB != particles.end(); ++itB) {
                P6Particle* a = *itA;
                P6Particle* b = *itB;

                glm::vec3 diff = a->position - b->position;
                float distSq = glm::dot(diff, diff);
                float radiusSum = a->radius + b->radius;
                float radiusSumSq = radiusSum * radiusSum;

                if (distSq <= radiusSumSq) {
                    float dist = sqrt(distSq);

                    ParticleContact* c = new ParticleContact();
                    c->particles[0] = a;
                    c->particles[1] = b;
                    c->penetration = radiusSum - dist;
                    c->contactNormal = glm::normalize(diff);
                    c->restitution = (a->restitution + b->restitution) * 0.5f;

                    AddContact(c);
                }
            }
        }

        // Generate contacts from links (rods, springs)
        for (auto* link : links) {
            ParticleContact* c = link->getContact();
            if (c) {
                std::cout << "Cable contact generated: penetration=" << c->penetration << "\n";
            }

            if (c) contacts.push_back(c);
        }
    }

    void PhysicsWorld::resolveContacts(float dt) {
        resolver.resolveContacts(contacts, dt);
    }
}
