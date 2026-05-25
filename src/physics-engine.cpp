#include "physics-engine.hpp"

sf::Vector2f PhysicsEngine::calculateForce(float mass1, sf::Vector2f position1, float mass2, sf::Vector2f position2) const noexcept{
    sf::Vector2f direction = (position2 - position1);
    float inv_len = Utils::fastInvSqrt(Utils::computeSquaredLength(direction)+0.1f);
    direction *= inv_len;
    return direction * G * mass1 * mass2 * (inv_len*inv_len);
}