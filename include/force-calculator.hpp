#include <SFML/Graphics.hpp>
#include <memory>
#include "particle-system.hpp"
#include "quad-tree.hpp"
#include "utils.hpp"

class IForceCalculator {
public:
    IForceCalculator() = default;
    virtual void calculateForces(const ParticleSystem& particles, std::vector<sf::Vector2f>& forces) = 0;
};

class BarnesHutCalculator : public IForceCalculator {
private:
    std::unique_ptr<QuadTree> quadTree;
    PhysicsEngine* physics;
    sf::Vector2u worldSize;
    float theta;
public:
    BarnesHutCalculator(sf::Vector2u worldSize, PhysicsEngine* physics, float theta);
    virtual void calculateForces(const ParticleSystem&, std::vector<sf::Vector2f>& forces);
    QuadTree* getQuadTree() const;
private:
    void calculateForceWithTree(
        const int targetIndex,
        const float* __restrict__ positions_x, 
        const float* __restrict__ positions_y, 
        const float* __restrict__ masses, 
        float thetaSquared,
        float& force_x,
        float& force_y
    ) const;
};