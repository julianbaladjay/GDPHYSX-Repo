#pragma once
#include <string>
#include <glm/glm.hpp>

class RaceResults
{
    private:
        std::string name;
        float magVelocity;
        glm::vec3 avgVelocity;
        float finishTime;

    public:
        RaceResults(const std::string& n, float magVel, glm::vec3 avgVel, float time);

        float getTime() const;
        std::string getName() const;

        void display(int rank) const;
};

