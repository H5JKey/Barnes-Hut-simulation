#include "physics-engine.hpp"
#include <iostream>

sf::Vector2f PhysicsEngine::calculateForce(const Particle& p1, const Particle& p2) const noexcept{
    sf::Vector2f direction = (p2.getPosition() - p1.getPosition());
    float len = computeLength(direction);
    len = fmax(2, len);
    direction /= len;
    return direction * G * p1.getMass() * p2.getMass() / (len*len);
}

sf::Vector2f PhysicsEngine::calculateForce(const Particle& p1, sf::Vector2f massCenterPosition, float totalMass) const noexcept{
    sf::Vector2f direction = (massCenterPosition - p1.getPosition());
    float squaredLen = computeSquaredLength(direction);
    direction /= fastSqrt(squaredLen);
    return direction * G * p1.getMass() * totalMass / (squaredLen+0.1f);
}

void PhysicsEngine::accelerate(Particle& p, sf::Vector2f force) noexcept{
    p.accelerate(force);
}

float PhysicsEngine::computeLength(sf::Vector2f vec) const noexcept{
    float len = fastSqrt(vec.x * vec.x + vec.y * vec.y);
    return len;
}

float PhysicsEngine::computeSquaredLength(sf::Vector2f vec) const noexcept{
    float len = vec.x * vec.x + vec.y * vec.y;
    return len;
}