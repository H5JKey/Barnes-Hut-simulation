#include "force-calculator.hpp"

BarnesHutCalculator::BarnesHutCalculator(sf::Vector2u worldSize, PhysicsEngine* physics, float theta) : worldSize(worldSize), physics(physics), theta(theta) {
    quadTree = std::make_unique<QuadTree>();
}

void BarnesHutCalculator::calculateForces(const ParticleSystem& particles, std::vector<sf::Vector2f>& forces) {
    forces.resize(particles.getCount());
    quadTree->rebuild(particles, worldSize);
    const float* __restrict__ positions_x = particles.getPositions_x().data();
    const float* __restrict__ positions_y = particles.getPositions_y().data();
    const float* __restrict__ masses = particles.getMasses().data();
    const float thetaSquared = theta*theta;
    #pragma omp parallel for schedule(dynamic, 128) 
    for (size_t i=0; i<particles.getCount(); ++i) {
        calculateForceWithTree(i, positions_x, positions_y, masses, thetaSquared, forces[i].x, forces[i].y);
    }
}

void BarnesHutCalculator::calculateForceWithTree(
    const int targetIndex, 
    const float* __restrict__ positions_x, 
    const float* __restrict__ positions_y, 
    const float* __restrict__ masses, 
    float thetaSquared,
    float& force_x,
    float& force_y
    ) const 
    {
    float targetMass = masses[targetIndex];
    float targetPosition_x = positions_x[targetIndex];
    float targetPosition_y = positions_y[targetIndex];

    //return calculateForceWithTree(targetIndex, targetPosition, targetMass, positions_x, positions_y, masses, theta*theta, 0);
    std::vector<int> stack;
    stack.reserve(256);
    stack.push_back(0);
    
    force_x = 0;
    force_y = 0;

    while (!stack.empty()) {
        int current = *stack.rbegin();
        stack.pop_back();
        const QuadTree::Node& node = quadTree->getNode(current);
        if (node.totalMass == 0) continue;
        if (node.isLeaf) {
            if (node.particleIndex == -1) continue;
            if (node.particleIndex == targetIndex) continue;

            float nodeMass = masses[node.particleIndex];
            float nodePosition_x = positions_x[node.particleIndex];
            float nodePosition_y = positions_y[node.particleIndex];
            
             float dx, dy;
             physics->calculateForce(
                targetMass,
                targetPosition_x,
                targetPosition_y,
                nodeMass, 
                nodePosition_x,
                nodePosition_y,
                dx,
                dy
            );
            force_x+=dx;
            force_y+=dy;
        }
        else {
            float squaredDistance = Utils::computeSquaredLength(targetPosition_x - node.centerOfMass.x, targetPosition_y - node.centerOfMass.y);
            float ratio = node.squaredSize / squaredDistance;
            if (ratio < thetaSquared) {
                float dx, dy;
                physics->calculateForce(targetMass, targetPosition_x, targetPosition_y, node.totalMass, node.centerOfMass.x, node.centerOfMass.y, dx, dy);
                force_x+=dx;
                force_y+=dy;
            }
            else {
                for (int child = 3; child >= 0; child--) {
                    if (node.children[child] != -1) 
                        stack.push_back(node.children[child]);
                }
            }
        }
    }
}

QuadTree* BarnesHutCalculator::getQuadTree() const {
    return quadTree.get();
}