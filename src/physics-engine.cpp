#include "physics-engine.hpp"
#include <iostream>

sf::Vector2f PhysicsEngine::calculateForce(float mass1, sf::Vector2f position1, float mass2, sf::Vector2f position2) const noexcept{
    sf::Vector2f direction = (position2 - position1);
    float inv_len = rsqrtf32(computeSquaredLength(direction)+0.1f);
    direction *= inv_len;
    return direction * G * mass1 * mass2 * (inv_len*inv_len);
}


float PhysicsEngine::computeLength(sf::Vector2f vec) const noexcept{
    float len = fastSqrt(vec.x * vec.x + vec.y * vec.y);
    return len;
}

float PhysicsEngine::computeSquaredLength(sf::Vector2f vec) const noexcept{
    float len = vec.x * vec.x + vec.y * vec.y;
    return len;
}