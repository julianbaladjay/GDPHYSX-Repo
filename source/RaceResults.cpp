#include "RaceResults.h"
#include <iostream>
#include <cmath>

// helper function
static std::string ordinal(int n) {
    if (n % 100 >= 11 && n % 100 <= 13) {
        return std::to_string(n) + "th";
    }
    switch (n % 10) {
    case 1: return std::to_string(n) + "st";
    case 2: return std::to_string(n) + "nd";
    case 3: return std::to_string(n) + "rd";
    default: return std::to_string(n) + "th";
    }
}

RaceResults::RaceResults(const std::string& n, float magVel, glm::vec3 avgVel, float time)
    : name(n), magVelocity(magVel), avgVelocity(avgVel), finishTime(time) {}

float RaceResults::getTime() const { return finishTime; }
std::string RaceResults::getName() const { return name; }

void RaceResults::display(int rank) const {
    std::cout << name << ": " << ordinal(rank) << "\n";
    std::cout << "Mag. of Velocity: "
        << std::round(magVelocity * 100.0f) / 100.0f << " m/s\n";
    std::cout << "Average Velocity: ("
        << std::round(avgVelocity.x * 100.0f) / 100.0f << ", "
        << std::round(avgVelocity.y * 100.0f) / 100.0f << ", "
        << std::round(avgVelocity.z * 100.0f) / 100.0f << ") m/s\n";
    std::cout << "Time: "
        << std::round(finishTime * 100.0f) / 100.0f << " secs\n\n";
}

