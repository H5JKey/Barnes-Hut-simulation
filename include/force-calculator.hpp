#include <SFML/Graphics.hpp>
#include <memory>
#include "particle-system.hpp"
#include "quad-tree.hpp"

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
    void updateCenterOfMass(const ParticleSystem& particles);
    sf::Vector2f calculateForceWithTree(int targetIndex, const ParticleSystem& particles, float theta);
    sf::Vector2f calculateForceWithTree(int targetIndex, const ParticleSystem& particles, float theta, int nodeIndex);
};