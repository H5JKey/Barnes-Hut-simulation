#pragma once
#include <vector>
#include <memory>
#include "particle-system.hpp"
#include <SFML/Graphics.hpp>
#include "physics-engine.hpp"
#include <optional>
#include <iostream>
#include <stack>


class QuadTree {
private:
    class Node {
    public:
        sf::Vector2f centerOfMass;
        sf::Vector2f center;
        float size;
        float totalMass;
        int children[4];
        int particleIndex;
        bool isLeaf;

        void insert(int particle_index, const ParticleSystem& particles);
        
        Node() = default;
        ~Node() = default;

        Node(sf::Vector2f center, double size) : isLeaf(true), center(center), size(size), particleIndex(-1), centerOfMass(0,0), totalMass(0) {
        }
    };
    std::vector<Node> nodes;
public:
    QuadTree();
    void rebuild( const ParticleSystem&, sf::Vector2u size);
    void insert(int targetPartilceIndex,  const ParticleSystem& particles);
    void draw(sf::RenderWindow& window) const;
    size_t size();
    Node& getNode(int i);
private:
    void insert(int targetPartilceIndex,  const ParticleSystem& particles, int nodeIndex);
};