#include "ForceRegistry.h"

namespace P6 {
    void ForceRegistry::add(P6Particle* p, ForceGenerator* g) {
        registrations.push_back({ p, g });
    }

    void ForceRegistry::remove(P6Particle* p, ForceGenerator* g) {
        registrations.remove_if([&](const ForceRegistration& reg) {
            return reg.particle == p && reg.generator == g;
            });
    }

    void ForceRegistry::clear() {
        registrations.clear();
    }

    void ForceRegistry::updateForces(float dt) {
        for (auto& reg : registrations) {
            reg.generator->updateForce(reg.particle, dt);
        }
    }
}
