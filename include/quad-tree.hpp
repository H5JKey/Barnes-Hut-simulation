#pragma once
#include <vector>
#include <memory>
#include "particle.hpp"
#include <SFML/Graphics.hpp>
#include "physics-engine.hpp"
#include <optional>
#include <iostream>
#include <stack>


class QuadTree {
private:
    class Node {
    public:
        int children[4];
        sf::Vector2f center;
        double size;
        bool isLeaf;
        const Particle *particle; 
        float totalMass;
        sf::Vector2f centerOfMass;
        

        void insert(const Particle& particle);
        
        Node() = default;
        ~Node() = default;

        Node(sf::Vector2f center, double size) : isLeaf(true), center(center), size(size), particle(), centerOfMass(0,0), totalMass(0) {
        }
    };
    std::vector<Node> nodes;
public:
    QuadTree(sf::Vector2u size);
    void rebuild(const std::vector<Particle>& particles, sf::Vector2u size);
    sf::Vector2f calculateForce(Particle& particle, float theta, const PhysicsEngine& physics);
    void insert(const Particle* particle);
    void updateCenterOfMass();
    void draw(sf::RenderWindow& window) const;
private:
    void insert(const Particle* particle, int node_index);
    sf::Vector2f calculateForce(const Particle& particle, int node_index, float theta, const PhysicsEngine& physics);
};