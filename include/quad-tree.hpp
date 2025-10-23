#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

class Particle {
public:
    sf::Vector2f position;
};


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
        Particle* particle;

        void insert(const Particle& particle);

        Node(sf::Vector2f center, double size) : isLeaf(true), center(center), size(size), particle(nullptr) {}
    };
    std::unique_ptr<Node> root;
public:
    QuadTree(sf::Vector2u size);
    
    void insert(Particle* particle);
private:
    void insert(Particle* particle, Node* node);

};