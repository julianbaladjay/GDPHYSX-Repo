#pragma once
#include <list>
#include <vector>
#include "P6Particle.h"
#include "ForceRegistry.h"
#include "ParticleContact.h"
#include "ContactResolver.h"
#include "ParticleLink.h" // base class for rods/springs

namespace P6 {
    class PhysicsWorld {
    public:
        // holds all particles
        std::list<P6Particle*> particles;

        // registry for forces
        ForceRegistry registry;

        // contacts for collisions/constraints
        std::vector<ParticleContact*> contacts;

        // resolver for contacts
        ContactResolver resolver;

        // links (rods, springs, etc.)
        std::vector<ParticleLink*> links;

        PhysicsWorld(unsigned maxIterations = 10);

        // add particles
        void AddParticle(P6Particle* toadd);

        // add links
        void AddLink(ParticleLink* link);

        // add contacts manually
        void AddContact(ParticleContact* contact);

        // world update
        void Update(float time);

    private:
        // update list of particles
        void UpdateParticleList();

        // generate contacts (collisions + links)
        void generateContacts();
        void resolveContacts(float dt);
    };
}
