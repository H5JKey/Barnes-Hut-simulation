#pragma once

#include <SFML/Graphics.hpp>
#include "particle.hpp"
#include <cmath>
#include "utils.h"

class PhysicsEngine {
private:
    float G;
public:
    PhysicsEngine() : G(667) {}

    sf::Vector2f calculateForce(const Particle& p1, const Particle& p2) const noexcept;
    sf::Vector2f calculateForce(const Particle& p1, sf::Vector2f massCenterPosition, float totalMass) const noexcept;
    void accelerate(Particle& p, sf::Vector2f force) noexcept;
    float computeLength(sf::Vector2f vec) const noexcept;
    float computeSquaredLength(sf::Vector2f vec) const noexcept;
};