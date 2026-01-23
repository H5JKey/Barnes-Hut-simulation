#include "force-calculator.hpp"

BarnesHutCalculator::BarnesHutCalculator(sf::Vector2u worldSize, PhysicsEngine* physics, float theta) : worldSize(worldSize), physics(physics), theta(theta) {
    quadTree = std::make_unique<QuadTree>();
}

void BarnesHutCalculator::calculateForces(const std::vector<Particle>& particles, std::vector<sf::Vector2f>& forces) {
    forces.resize(particles.size());
    quadTree->rebuild(particles, worldSize);
    updateCenterOfMass(particles);
    #pragma omp parallel for
    for (int i=0; i<particles.size(); i++) {
        forces[i] = calculateForceWithTree(i, particles, theta);
    }
}


void BarnesHutCalculator::updateCenterOfMass(const std::vector<Particle>& particles) {
    for (int i=quadTree->size() - 1; i >= 0; i--) {
        if (quadTree->getNode(i).isLeaf) {
            if (quadTree->getNode(i).particleIndex != -1) {
                quadTree->getNode(i).centerOfMass = particles[quadTree->getNode(i).particleIndex].getPosition();
                quadTree->getNode(i).totalMass = particles[quadTree->getNode(i).particleIndex].getMass();
            }
            else {
                quadTree->getNode(i).totalMass = 0;
                quadTree->getNode(i).centerOfMass = {0, 0};
            }
        }
        else {
            for (int child = 0; child < 4; child++) {
                quadTree->getNode(i).centerOfMass+=quadTree->getNode(quadTree->getNode(i).children[child]).centerOfMass*quadTree->getNode(quadTree->getNode(i).children[child]).totalMass;
                quadTree->getNode(i).totalMass+=quadTree->getNode(quadTree->getNode(i).children[child]).totalMass;
            }
            quadTree->getNode(i).centerOfMass/=quadTree->getNode(i).totalMass;
        }
    }
}

sf::Vector2f BarnesHutCalculator::calculateForceWithTree(int targetIndex, const std::vector<Particle>& particles, float theta) {
    return calculateForceWithTree(targetIndex, particles, theta, 0);
}

sf::Vector2f BarnesHutCalculator::calculateForceWithTree(int targetIndex, const std::vector<Particle>& particles, float theta, int nodeIndex) {
    if (quadTree->getNode(nodeIndex).totalMass == 0) {
        return {0,0};
    }
    if (quadTree->getNode(nodeIndex).isLeaf) {
        if (quadTree->getNode(nodeIndex).particleIndex != -1 && particles[quadTree->getNode(nodeIndex).particleIndex].getPosition() != particles[targetIndex].getPosition()) {
            return physics->calculateForce(particles[targetIndex], particles[quadTree->getNode(nodeIndex).particleIndex]);
        }
        return {0,0};
    } 
    else {
        float squaredDistance = physics->computeSquaredLength(particles[targetIndex].getPosition() - quadTree->getNode(nodeIndex).centerOfMass);
        float ratio = quadTree->getNode(nodeIndex).size * quadTree->getNode(nodeIndex).size / squaredDistance;
        
        if (ratio < theta*theta) {
            return physics->calculateForce(particles[targetIndex], quadTree->getNode(nodeIndex).centerOfMass, quadTree->getNode(nodeIndex).totalMass);
        }
        else {
            sf::Vector2f totalForce(0, 0);
            for (int child = 0; child < 4; child++) {
                totalForce += calculateForceWithTree(targetIndex, particles,  theta, quadTree->getNode(nodeIndex).children[child]);
            }
            return totalForce;
        }
    }
    return {0,0};
}

QuadTree* BarnesHutCalculator::getQuadTree() const {
    return quadTree.get();
}