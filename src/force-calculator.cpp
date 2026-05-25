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
    const float thetaSquared = theta*theta;
    #pragma omp parallel for schedule(dynamic, 64) 
    for (size_t i=0; i<particles.getCount(); ++i) {
        forces[i] = calculateForceWithTree(i, positions_x, positions_y, masses, thetaSquared);
    }
}

sf::Vector2f BarnesHutCalculator::calculateForceWithTree(
    const int targetIndex, 
    const std::vector<float>& positions_x, 
    const std::vector<float>& positions_y, 
    const std::vector<float>& masses, 
    float thetaSquared
    ) const 
    {
    float targetMass = masses[targetIndex];
    sf::Vector2f targetPosition = sf::Vector2f(positions_x[targetIndex], positions_y[targetIndex]);

    //return calculateForceWithTree(targetIndex, targetPosition, targetMass, positions_x, positions_y, masses, theta*theta, 0);
    std::vector<int> stack;
    stack.push_back(0);
    
    sf::Vector2f totalForce{0,0};

    while (!stack.empty()) {
        int current = *stack.rbegin();
        stack.pop_back();
        const QuadTree::Node& node = quadTree->getNode(current);
        if (node.totalMass == 0) continue;
        if (node.isLeaf) {
            if (node.particleIndex == -1) continue;
            if (node.particleIndex == targetIndex) continue;

            float nodeMass = masses[node.particleIndex];
            sf::Vector2f nodePosition = sf::Vector2f(positions_x[node.particleIndex], positions_y[node.particleIndex]);

            totalForce+=physics->calculateForce(
                targetMass,
                targetPosition, 
                nodeMass, 
                nodePosition
            );
        }
        else {
            float squaredDistance = Utils::computeSquaredLength(targetPosition - node.centerOfMass);
            float ratio = node.squaredSize / squaredDistance;
            if (ratio < thetaSquared) {
                totalForce+=physics->calculateForce(targetMass, targetPosition, node.totalMass, node.centerOfMass);
            }
            else {
                for (int child = 0; child < 4; child++) {
                    if (node.children[child] != -1) 
                        stack.push_back(node.children[child]);
                }
            }
        }
    }
    return totalForce;
}

QuadTree* BarnesHutCalculator::getQuadTree() const {
    return quadTree.get();
}