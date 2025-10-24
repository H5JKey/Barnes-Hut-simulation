#include <vector>
#include <memory>
#include "particle.hpp"
#include <SFML/Graphics.hpp>


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
        const Particle* particle;
        double totalMass;
        sf::Vector2f centerOfMass;
        

        void insert(const Particle& particle);

        Node(sf::Vector2f center, double size) : isLeaf(true), center(center), size(size), particle(nullptr), centerOfMass(0,0), totalMass(0) {}
    };
    std::unique_ptr<Node> root;
public:
    QuadTree(sf::Vector2u size);
    void insert(const Particle* particle);
    void rebuild(const std::vector<Particle>& particles);

private:
    void insert(const Particle* particle, Node* node);

};