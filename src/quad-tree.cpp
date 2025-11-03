#include "quad-tree.hpp"
#include <iostream>

void QuadTree::insert(const Particle* particle) {
    insert(particle, root.get());
}

void QuadTree::insert(const Particle* particle, Node* node, int depth) {
    if (depth>20) {
        return;
    }
    if (node->isLeaf && !node->particle) {
            node->particle = particle;
            return;
    }
    if (node->isLeaf && node->particle) {
        const Particle *oldParticle = node->particle;
        node->isLeaf = false;
        
        node->northWest = std::make_unique<Node>(
            sf::Vector2f(node->center.x - node->size / 4.f, node->center.y - node->size / 4.f),
            node->size / 2.f
        );

        node->northEast = std::make_unique<Node>(
            sf::Vector2f(node->center.x + node->size / 4.f, node->center.y - node->size / 4.f),
            node->size / 2.f
        );
        
        node->southWest = std::make_unique<Node>(
            sf::Vector2f(node->center.x - node->size / 4.f, node->center.y + node->size / 4.f),
            node->size / 2.f
        );
        
        node->southEast = std::make_unique<Node>(
            sf::Vector2f(node->center.x + node->size / 4.f, node->center.y + node->size / 4.f),
            node->size / 2.f
        );
        insert(oldParticle, node, depth + 1);
        insert(particle, node, depth + 1);
        return;
    }

    if (!node->isLeaf) {
        float x = particle->getPosition().x;
        float y = particle->getPosition().y;
        if (x < node->center.x && y < node->center.y) {
            insert(particle, node->northWest.get(), depth + 1);
        }
        else if (x >= node->center.x && y < node->center.y)  {
            insert(particle, node->northEast.get(), depth + 1);
        }
        else if (x < node->center.x && y >= node->center.y) {
            insert(particle, node->southWest.get(), depth + 1);
        }
        else {
            insert(particle, node->southEast.get(), depth + 1);
        }
    }
}

QuadTree::QuadTree(sf::Vector2u size) {
    root = std::make_unique<Node>(static_cast<sf::Vector2f>(size) / 2.0f, std::max(size.x, size.y));
}

void QuadTree::rebuild(const std::vector<Particle>& particles) {
    root = std::make_unique<Node>(root->center, root->size);
    for (const Particle& p: particles) {
        insert(&p);
    }
    updateCenterOfMass();
}

void QuadTree::updateCenterOfMass(Node* node) {
    float totalMass = 0;
    sf::Vector2f centerOfMass = {0,0};
    if (node->isLeaf) {
        if (node->particle) {
            node->totalMass = node->particle->getMass();
            node->centerOfMass = node->particle->getPosition();
        } else {
            node->totalMass = 0;
            node->centerOfMass = {0, 0};
        }
        return;
    }
    updateCenterOfMass(node->northEast.get());
    totalMass+=node->northEast->totalMass;
    centerOfMass+=node->northEast->centerOfMass * node->northEast->totalMass;

    updateCenterOfMass(node->northWest.get());
    totalMass+=node->northWest->totalMass;
    centerOfMass+=node->northWest->centerOfMass * node->northWest->totalMass;

    updateCenterOfMass(node->southEast.get());
    totalMass+=node->southEast->totalMass;
    centerOfMass+=node->southEast->centerOfMass * node->southEast->totalMass;

    updateCenterOfMass(node->southWest.get());
    totalMass+=node->southWest->totalMass;
    centerOfMass+=node->southWest->centerOfMass * node->southWest->totalMass;
    if (totalMass > 0)
        node->centerOfMass = centerOfMass / totalMass;
    else
        node->centerOfMass = {0,0};
    node->totalMass = totalMass;

}

void QuadTree::updateCenterOfMass() {
    updateCenterOfMass(root.get());
}

sf::Vector2f QuadTree::calculateForce(Particle& particle, float theta, const PhysicsEngine& physics) {
    return calculateForce(particle, root.get(), theta, physics);
}


sf::Vector2f QuadTree::calculateForce(const Particle& particle, Node* node, float theta, const PhysicsEngine& physics) {
    if (node->totalMass == 0) {
        return {0,0};
    }
    if (node->isLeaf) {
        if (node->particle && node->particle->getPosition() != particle.getPosition()) {
            return physics.calculateForce(particle, *node->particle);
        }
        return {0,0};
    } 
    else {
        float squaredDistance = physics.computeSquaredLength(particle.getPosition() - node->centerOfMass);
        if (squaredDistance < 1e-10f) {
            return {0,0};
        }
        float ratio = node->size * node->size / squaredDistance;
        
        if (ratio < theta*theta) {
            return physics.calculateForce(particle, node->centerOfMass, node->totalMass);
        }
        else {
            sf::Vector2f totalForce(0, 0);
            if (node->northWest) totalForce += calculateForce(particle, node->northWest.get(), theta, physics);
            if (node->northEast) totalForce += calculateForce(particle, node->northEast.get(), theta, physics);
            if (node->southWest) totalForce += calculateForce(particle, node->southWest.get(), theta, physics);
            if (node->southEast) totalForce += calculateForce(particle, node->southEast.get(), theta, physics);
            return totalForce;
        }
    }
    return {0,0};
}


void QuadTree::draw(sf::RenderWindow& window) const {
    draw(window, root.get());
}

void QuadTree::draw(sf::RenderWindow& window, Node* node) const {
    if (!node) return;

    sf::RectangleShape border(sf::Vector2f(node->size, node->size));
    border.setPosition(node->center.x - node->size / 2, node->center.y - node->size / 2);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::White);
    border.setOutlineThickness(1);
    border.setOutlineColor(sf::Color(255,255,255,30));
    window.draw(border);


    draw(window, node->northWest.get());
    draw(window, node->northEast.get());
    draw(window, node->southWest.get());
    draw(window, node->southEast.get());
}
