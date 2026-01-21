#include "quad-tree.hpp"
#include <iostream>

void QuadTree::insert(int particle_index, const std::vector<Particle>& particles) {
    insert(particle_index, 0, particles);
}

void QuadTree::insert(int particle_index, int node_index, const std::vector<Particle>& particles) {
    for (int i=0; i<50; i++) {
        if (nodes[node_index].isLeaf && nodes[node_index].particleIndex == -1) {
                nodes[node_index].particleIndex = particle_index;
                return;
        }
        if (nodes[node_index].isLeaf && nodes[node_index].particleIndex != -1) {
            int oldParticleIndex = nodes[node_index].particleIndex;
            nodes[node_index].isLeaf = false;
            
            nodes[node_index].children[0] = nodes.size();
            nodes.emplace_back(
                sf::Vector2f(nodes[node_index].center.x - nodes[node_index].size / 4.f, nodes[node_index].center.y - nodes[node_index].size / 4.f),
                nodes[node_index].size / 2.f
            );

            nodes[node_index].children[1] = nodes.size();
            nodes.emplace_back(
                sf::Vector2f(nodes[node_index].center.x + nodes[node_index].size / 4.f, nodes[node_index].center.y - nodes[node_index].size / 4.f),
                nodes[node_index].size / 2.f
            );
            
            nodes[node_index].children[2] = nodes.size();
            nodes.emplace_back(
                sf::Vector2f(nodes[node_index].center.x - nodes[node_index].size / 4.f, nodes[node_index].center.y + nodes[node_index].size / 4.f),
                nodes[node_index].size / 2.f
            );
            
            nodes[node_index].children[3] = nodes.size();
            nodes.emplace_back(
                sf::Vector2f(nodes[node_index].center.x + nodes[node_index].size / 4.f, nodes[node_index].center.y + nodes[node_index].size / 4.f),
                nodes[node_index].size / 2.f
            );
            insert(oldParticleIndex, node_index, particles);
            continue;
        }

        if (!nodes[node_index].isLeaf) {
            float x = particles[particle_index].getPosition().x;
            float y = particles[particle_index].getPosition().y;
            if (x < nodes[node_index].center.x && y < nodes[node_index].center.y) {
                node_index = nodes[node_index].children[0];
            }
            else if (x >= nodes[node_index].center.x && y < nodes[node_index].center.y)  {
                node_index = nodes[node_index].children[1];
            }
            else if (x < nodes[node_index].center.x && y >= nodes[node_index].center.y) {
                node_index = nodes[node_index].children[2];
            }
            else {
                node_index = nodes[node_index].children[3];
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

sf::Vector2f QuadTree::calculateForce(Particle& particle, float theta, const PhysicsEngine& physics, const std::vector<Particle>& particles) {
    return calculateForce(particle, 0, theta, physics, particles);
}


sf::Vector2f QuadTree::calculateForce(const Particle& particle, int node_index, float theta, const PhysicsEngine& physics, const std::vector<Particle>& particles) {
    if (nodes[node_index].totalMass == 0) {
        return {0,0};
    }
    if (nodes[node_index].isLeaf) {
        if (nodes[node_index].particleIndex != -1 && particles[nodes[node_index].particleIndex].getPosition() != particle.getPosition()) {
            return physics.calculateForce(particle, particles[nodes[node_index].particleIndex]);
        }
        return {0,0};
    } 
    else {
        float squaredDistance = physics.computeSquaredLength(particle.getPosition() - nodes[node_index].centerOfMass);
        float ratio = nodes[node_index].size * nodes[node_index].size / squaredDistance;
        
        if (ratio < theta*theta) {
            return physics.calculateForce(particle, nodes[node_index].centerOfMass, nodes[node_index].totalMass);
        }
        else {
            sf::Vector2f totalForce(0, 0);
            for (int child = 0; child < 4; child++) {
                totalForce += calculateForce(particle, nodes[node_index].children[child], theta, physics, particles);
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
