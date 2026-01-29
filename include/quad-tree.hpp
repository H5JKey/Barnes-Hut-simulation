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
        float squaredSize;
        float totalMass;
        int children[4];
        int particleIndex;
        bool isLeaf;
        
        Node() = default;
        ~Node() = default;

        Node(sf::Vector2f center, double size) : isLeaf(true), center(center), size(size), squaredSize(size*size), particleIndex(-1), centerOfMass(0,0), totalMass(0) {
        }
    };
    std::vector<Node> nodes;
public:
    QuadTree();
    void rebuild(const ParticleSystem&, sf::Vector2u size);
    void draw(sf::RenderWindow& window) const;
    size_t size();
    Node& getNode(int i);
private:
    int buildQuadTree(const ParticleSystem& particles, std::vector<int>::iterator begin, std::vector<int>::iterator end, sf::Vector2f center, float size);
};