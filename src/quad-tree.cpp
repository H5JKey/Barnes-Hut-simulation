#include "quad-tree.hpp"
#include <iostream>

void QuadTree::insert(const Particle* particle) {
    insert(particle, 0);
}

void QuadTree::insert(const Particle* particle, int node_index) {
    for (int i=0; i<50; i++) {
        if (nodes[node_index].isLeaf && !nodes[node_index].particle) {
                nodes[node_index].particle = particle;
                return;
        }
        if (nodes[node_index].isLeaf && nodes[node_index].particle) {
            const Particle *oldParticle = nodes[node_index].particle;
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
            insert(oldParticle, node_index);
            continue;
        }

        if (!nodes[node_index].isLeaf) {
            float x = particle->getPosition().x;
            float y = particle->getPosition().y;
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
    for (const Particle& p: particles) {
        insert(&p);
    }
    updateCenterOfMass();
}


void QuadTree::updateCenterOfMass() {
    for (int i=nodes.size() - 1; i >= 0; i--) {
        if (nodes[i].isLeaf) {
            if (nodes[i].particle) {
                nodes[i].centerOfMass = nodes[i].particle->getPosition();
                nodes[i].totalMass = nodes[i].particle->getMass();
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

sf::Vector2f QuadTree::calculateForce(Particle& particle, float theta, const PhysicsEngine& physics) {
    return calculateForce(particle, 0, theta, physics);
}


sf::Vector2f QuadTree::calculateForce(const Particle& particle, int node_index, float theta, const PhysicsEngine& physics) {
    if (nodes[node_index].totalMass == 0) {
        return {0,0};
    }
    if (nodes[node_index].isLeaf) {
        if (nodes[node_index].particle && nodes[node_index].particle->getPosition() != particle.getPosition()) {
            return physics.calculateForce(particle, *nodes[node_index].particle);
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
                totalForce += calculateForce(particle, nodes[node_index].children[child], theta, physics);
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
