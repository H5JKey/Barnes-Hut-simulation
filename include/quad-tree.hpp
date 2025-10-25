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
        std::unique_ptr<Node> northWest;
        std::unique_ptr<Node> northEast;
        std::unique_ptr<Node> southWest;
        std::unique_ptr<Node> southEast;

        sf::Vector2f center;
        double size;
        bool isLeaf;
        std::optional<Particle> particle; 
        float totalMass;
        sf::Vector2f centerOfMass;
        

        void insert(const Particle& particle);

        Node(sf::Vector2f center, double size) : isLeaf(true), center(center), size(size), particle(), centerOfMass(0,0), totalMass(0) {}
    };
    std::unique_ptr<Node> root;
public:
    QuadTree(sf::Vector2u size);
    void rebuild(const std::vector<Particle>& particles);
    sf::Vector2f calculateForce(Particle& particle, float theta, const PhysicsEngine& physics);
    void insert(const Particle& particle);
    void updateCenterOfMass();
    void draw(sf::RenderWindow& window) const;
private:
    void updateCenterOfMass(Node* node);
    void insert(const Particle& particle, Node*node);
    sf::Vector2f calculateForce(const Particle& particle, Node* node, float theta, const PhysicsEngine& physics);

    void draw(sf::RenderWindow& window, Node* node) const;
};