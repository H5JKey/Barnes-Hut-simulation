#include "quad-tree.hpp"


void QuadTree::insert(Particle* particle) {
    insert(particle, root.get());
}

void QuadTree::insert(Particle* particle, Node* node) {
    if (node->isLeaf && node->particle == nullptr) {
            node->particle = particle;
            return;
    }
    if (node->isLeaf && node->particle != nullptr) {
            Particle* oldParticle = node->particle;
            node->particle = nullptr;
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
                
            insert(oldParticle, node);
            insert(particle, node);
            return;
        }

        if (!node->isLeaf) {
            float x = particle->position.x;
            float y = particle->position.y;

            if (x < node->center.x && y < node->center.y) 
                insert(particle, node->northWest.get());
            else if (x >= node->center.x && y < node->center.y) 
                insert(particle, node->northEast.get());
            else if (x < node->center.x && y >= node->center.y) 
                insert(particle, node->southWest.get());
            else
                insert(particle, node->southEast.get());
        }
}

QuadTree::QuadTree(sf::Vector2u size) {
    root = std::make_unique<Node>(static_cast<sf::Vector2f>(size) / 2.0f, std::max(size.x, size.y));
}