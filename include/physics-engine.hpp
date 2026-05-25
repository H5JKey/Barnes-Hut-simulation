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

    void calculateForce(float mass1, float position1_x, float position1_y, float mass2, float position2_x, float position2_y, float& force_x, float& force_y) const noexcept;
};