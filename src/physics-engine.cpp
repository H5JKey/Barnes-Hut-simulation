#include "physics-engine.hpp"

void PhysicsEngine::calculateForce(float mass1, float position1_x, float position1_y, float mass2, float position2_x, float position2_y, float& force_x, float& force_y) const noexcept {
    float direction_x = position2_x - position1_x;
    float direction_y = position2_y - position1_y;
    float inv_len = Utils::fastInvSqrt(Utils::computeSquaredLength(direction_x, direction_y)+100.f);
    float magnitude = mass1 * mass2 * G * inv_len * inv_len * inv_len;
    direction_x *= magnitude;
    direction_y *= magnitude;
    force_x = direction_x;
    force_y = direction_y;
}
