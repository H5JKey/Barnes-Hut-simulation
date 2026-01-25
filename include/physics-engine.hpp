#pragma once

#include <SFML/Graphics.hpp>
#include "particle-system.hpp"
#include <cmath>
#include "utils.hpp"

class PhysicsEngine {
private:
    float G;
public:
    PhysicsEngine() : G(667) {}

    sf::Vector2f calculateForce(float mass1, sf::Vector2f position1, float mass2, sf::Vector2f position2) const noexcept;
    float computeLength(sf::Vector2f vec) const noexcept;
    float computeSquaredLength(sf::Vector2f vec) const noexcept;
};