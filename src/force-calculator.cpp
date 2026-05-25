#include "force-calculator.hpp"

BarnesHutCalculator::BarnesHutCalculator(sf::Vector2u worldSize, PhysicsEngine* physics, float theta) : worldSize(worldSize), physics(physics), theta(theta) {
    quadTree = std::make_unique<QuadTree>();
}

void BarnesHutCalculator::calculateForces(const ParticleSystem& particles, std::vector<sf::Vector2f>& forces) {
    forces.resize(particles.getCount());
    quadTree->rebuild(particles, worldSize);
    const auto& positions_x = particles.getPositions_x();
    const auto& positions_y = particles.getPositions_y();
    const auto& masses = particles.getMasses();
    #pragma omp parallel for
    for (int i=0; i<particles.getCount(); i++) {
        forces[i] = calculateForceWithTree(i, positions_x, positions_y, masses, theta);
    }
}

sf::Vector2f BarnesHutCalculator::calculateForceWithTree(int targetIndex, const std::vector<float>& positions_x, const std::vector<float>& positions_y, const std::vector<float>& masses, float theta) {
    return calculateForceWithTree(targetIndex, positions_x, positions_y, masses, theta, 0);
}

sf::Vector2f BarnesHutCalculator::calculateForceWithTree(int targetIndex, const std::vector<float>& positions_x, const std::vector<float>& positions_y, const std::vector<float>& masses, float theta, int nodeIndex) {
    if (quadTree->getNode(nodeIndex).totalMass == 0) {
        return {0,0};
    }

    float targetMass = masses[targetIndex];
    sf::Vector2f targetPosition = sf::Vector2f(positions_x[targetIndex], positions_y[targetIndex]);

    if (quadTree->getNode(nodeIndex).isLeaf) {
        if (quadTree->getNode(nodeIndex).particleIndex == -1) return {0,0};

        float nodeMass = masses[quadTree->getNode(nodeIndex).particleIndex];
        sf::Vector2f nodePosition = sf::Vector2f(positions_x[quadTree->getNode(nodeIndex).particleIndex], positions_y[quadTree->getNode(nodeIndex).particleIndex]);

        if (nodePosition != targetPosition) {
            return physics->calculateForce(targetMass,
                                           targetPosition, 
                                           nodeMass, 
                                           nodePosition
                                        );
        }
        return {0,0};
    } 
    else {
        float squaredDistance = physics->computeSquaredLength(targetPosition - quadTree->getNode(nodeIndex).centerOfMass);
        float ratio = quadTree->getNode(nodeIndex).squaredSize / squaredDistance;
        if (ratio < theta*theta) {
            return physics->calculateForce(targetMass, targetPosition, quadTree->getNode(nodeIndex).totalMass, quadTree->getNode(nodeIndex).centerOfMass);
        }
        else {
            sf::Vector2f totalForce(0, 0);
            for (int child = 0; child < 4; child++) {
                totalForce += calculateForceWithTree(targetIndex, positions_x, positions_y, masses,  theta, quadTree->getNode(nodeIndex).children[child]);
            }
            return totalForce;
        }
    }
    return {0,0};
}

QuadTree* BarnesHutCalculator::getQuadTree() const {
    return quadTree.get();
}