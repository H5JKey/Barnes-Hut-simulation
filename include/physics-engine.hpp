#pragma once

#include <SFML/Graphics.hpp>
#include "particle.hpp"
#include <cmath>

class PhysicsEngine {
private:
    float G;
public:
    PhysicsEngine() : G(667) {}

    sf::Vector2f calculateForce(const Particle& p1, const Particle& p2) const;
    sf::Vector2f calculateForce(const Particle& p1, sf::Vector2f massCenterPosition, float totalMass) const;
    void accelerate(Particle& p, sf::Vector2f force);
    float computeLength(sf::Vector2f vec) const;
    float computeSquaredLength(sf::Vector2f vec) const;
};