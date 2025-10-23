#include <SFML/Graphics.hpp>
#include "quad-tree.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "QuadTree Visualization");
    
    QuadTree tree(sf::Vector2u(800, 800));
    

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear();

        window.display();
    }
    
    return 0;
}