#pragma once
#include "ParticleLink.h"
#include "ParticleContact.h"

namespace P6 {
    class Rod : public ParticleLink {
    public:
        float rodLength;

        Rod(float length);
        ParticleContact* getContact() const override;
    };
}
