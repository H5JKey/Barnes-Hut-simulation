#include <SFML/Graphics.hpp>
#include "quad-tree.hpp"
#include "particle.hpp"
#include <random>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "QuadTree Visualization");
    
    QuadTree tree(sf::Vector2u(800, 800));
    std::vector<Particle> particles;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posDist(0.0f, 800.0f);
    std::uniform_real_distribution<float> massDist(0.1f, 5.0f);
    
    const int numParticles = 10000;
    for (int i = 0; i < numParticles; ++i) {
        sf::Vector2f randomPos(posDist(gen), posDist(gen));
        float randomMass = massDist(gen);
        particles.emplace_back(randomPos, randomMass);
    }
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();            
        }
        
        tree.rebuild(particles);

        window.clear();
        for (auto& p : particles) {
            p.draw(window);
        }
        window.display();
    }
    
    return 0;
}