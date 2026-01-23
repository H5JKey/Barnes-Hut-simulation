#include "quad-tree.hpp"
#include <iostream>

void QuadTree::insert(int targetPartilceIndex, const std::vector<Particle>& particles) {
    insert(targetPartilceIndex, particles, 0);
}

void QuadTree::insert(int targetPartilceIndex, const std::vector<Particle>& particles, int nodeIndex) {
    int depth = 0;
    int MAX_DEPTH = 50;
    while (true) {
        depth++;
        if (depth > MAX_DEPTH) {
            nodes[nodeIndex].particleIndex = targetPartilceIndex;
            return;
        }

        if (nodes[nodeIndex].isLeaf) {
            if (nodes[nodeIndex].particleIndex == -1) {
                nodes[nodeIndex].particleIndex = targetPartilceIndex;
                return;
            }
            int oldParticleIndex = nodes[nodeIndex].particleIndex;
            nodes[nodeIndex].isLeaf = false;

            nodes[nodeIndex].children[0] = nodes.size();
            nodes.emplace_back(
                sf::Vector2f(nodes[nodeIndex].center.x - nodes[nodeIndex].size / 4.f, nodes[nodeIndex].center.y - nodes[nodeIndex].size / 4.f),
                nodes[nodeIndex].size / 2.f
            );

            nodes[nodeIndex].children[1] = nodes.size();
            nodes.emplace_back(
                sf::Vector2f(nodes[nodeIndex].center.x + nodes[nodeIndex].size / 4.f, nodes[nodeIndex].center.y - nodes[nodeIndex].size / 4.f),
                nodes[nodeIndex].size / 2.f
            );

            nodes[nodeIndex].children[2] = nodes.size();
            nodes.emplace_back(
                sf::Vector2f(nodes[nodeIndex].center.x - nodes[nodeIndex].size / 4.f, nodes[nodeIndex].center.y + nodes[nodeIndex].size / 4.f),
                nodes[nodeIndex].size / 2.f
            );

            nodes[nodeIndex].children[3] = nodes.size();
            nodes.emplace_back(
                sf::Vector2f(nodes[nodeIndex].center.x + nodes[nodeIndex].size / 4.f, nodes[nodeIndex].center.y + nodes[nodeIndex].size / 4.f),
                nodes[nodeIndex].size / 2.f
            );
            insert(oldParticleIndex, particles, nodeIndex);
            continue;


        }
        else {
            float x = particles[targetPartilceIndex].getPosition().x;
            float y = particles[targetPartilceIndex].getPosition().y;
            if (x < nodes[nodeIndex].center.x && y < nodes[nodeIndex].center.y) {
                nodeIndex = nodes[nodeIndex].children[0];
            }
            else if (x >= nodes[nodeIndex].center.x && y < nodes[nodeIndex].center.y)  {
                nodeIndex = nodes[nodeIndex].children[1];
            }
            else if (x < nodes[nodeIndex].center.x && y >= nodes[nodeIndex].center.y) {
                nodeIndex = nodes[nodeIndex].children[2];
            }
            else {
                nodeIndex = nodes[nodeIndex].children[3];
            }
        }
    }
}

QuadTree::QuadTree(sf::Vector2u size) {
    nodes.resize(0);
    nodes.reserve(10000);
    nodes.emplace_back(static_cast<sf::Vector2f>(size) / 2.0f, std::max(size.x, size.y));
}

void QuadTree::rebuild(const std::vector<Particle>& particles, sf::Vector2u size) {
    nodes.resize(0);
    nodes.reserve(particles.size());
    nodes.emplace_back(static_cast<sf::Vector2f>(size) / 2.0f, std::max(size.x, size.y));
    for (int i=0; i<particles.size(); i++) {
        insert(i, particles);
    }
    updateCenterOfMass(particles);
}


void QuadTree::updateCenterOfMass(const std::vector<Particle>& particles) {
    for (int i=nodes.size() - 1; i >= 0; i--) {
        if (nodes[i].isLeaf) {
            if (nodes[i].particleIndex != -1) {
                nodes[i].centerOfMass = particles[nodes[i].particleIndex].getPosition();
                nodes[i].totalMass = particles[nodes[i].particleIndex].getMass();
            }
            else {
                nodes[i].totalMass = 0;
                nodes[i].centerOfMass = {0, 0};
            }
        }
        else {
            for (int child = 0; child < 4; child++) {
                nodes[i].centerOfMass+=nodes[nodes[i].children[child]].centerOfMass*nodes[nodes[i].children[child]].totalMass;
                nodes[i].totalMass+=nodes[nodes[i].children[child]].totalMass;
            }
            nodes[i].centerOfMass/=nodes[i].totalMass;
        }
    }
}

sf::Vector2f QuadTree::calculateForce(int targetParticleIndex, const std::vector<Particle>& particles, float theta, const PhysicsEngine& physics) {
    return calculateForce(targetParticleIndex, particles, 0, theta, physics);
}


sf::Vector2f QuadTree::calculateForce(int targetParticleIndex, const std::vector<Particle>& particles, int nodeIndex, float theta, const PhysicsEngine& physics) {
    if (nodes[nodeIndex].totalMass == 0) {
        return {0,0};
    }
    if (nodes[nodeIndex].isLeaf) {
        if (nodes[nodeIndex].particleIndex != -1 && particles[nodes[nodeIndex].particleIndex].getPosition() != particles[targetParticleIndex].getPosition()) {
            return physics.calculateForce(particles[targetParticleIndex], particles[nodes[nodeIndex].particleIndex]);
        }
        return {0,0};
    } 
    else {
        float squaredDistance = physics.computeSquaredLength(particles[targetParticleIndex].getPosition() - nodes[nodeIndex].centerOfMass);
        float ratio = nodes[nodeIndex].size * nodes[nodeIndex].size / squaredDistance;
        
        if (ratio < theta*theta) {
            return physics.calculateForce(particles[targetParticleIndex], nodes[nodeIndex].centerOfMass, nodes[nodeIndex].totalMass);
        }
        else {
            sf::Vector2f totalForce(0, 0);
            for (int child = 0; child < 4; child++) {
                totalForce += calculateForce(targetParticleIndex, particles, nodes[nodeIndex].children[child], theta, physics);
            }
            return totalForce;
        }
    }
    return {0,0};
}


void QuadTree::draw(sf::RenderWindow& window) const {
    sf::RectangleShape border;
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::White);
    border.setOutlineThickness(1);
    border.setOutlineColor(sf::Color(255,255,255,30));

    for (int i=0; i<nodes.size(); i++) {
        border.setSize(sf::Vector2f(nodes[i].size, nodes[i].size));
        border.setPosition(nodes[i].center.x - nodes[i].size / 2, nodes[i].center.y - nodes[i].size / 2);
        window.draw(border);
    }
}
