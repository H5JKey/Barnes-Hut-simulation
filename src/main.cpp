#include <SFML/Graphics.hpp>
#include "quad-tree.hpp"
#include "particle.hpp"
#include "physics-engine.hpp"
#include <random>
#include <cmath>
#include <vector>

#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Galaxy Simulation");
    
    QuadTree tree(sf::Vector2u(800, 800));
    PhysicsEngine physics;
    
    std::vector<Particle> particles;
    for (float phi=0; phi<6.28; phi+=0.005) {
        float x = 400 + 100*cos(phi);
        float y = 400 + 100*sin(phi);
        particles.emplace_back(sf::Vector2f(x, y), 1);
        sf::Vector2f velocity(100*sin(phi), 100*cos(phi));
        particles.back().setVelocity(velocity);
    }
    particles.emplace_back(sf::Vector2f(400, 400), 1000);
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        sf::Time elapsedTime = clock.restart();
        tree.rebuild(particles);
        if (elapsedTime.asSeconds() > 0.033f) {
            elapsedTime = sf::seconds(0.033f);
        }
        particles.erase(
                std::remove_if(
                    particles.begin(),
                    particles.end(),
                    [&](const Particle& p) {
                        return p.getPosition().x < 0 || p.getPosition().x > window.getSize().x ||
                            p.getPosition().y < 0 || p.getPosition().y > window.getSize().y;
                    }),
                particles.end()
            );
        for (auto& p : particles) {
            sf::Vector2f force = tree.calculateForce(p, 0.5f, physics);
            //std::cout<<force.x << ' '<< force.y<<'\n';
            physics.accelerate(p, force);
            p.update(elapsedTime);
        }
        
        window.clear();
        for (auto& p : particles) {
            p.draw(window);
        }
        tree.draw(window);
        window.display();
    }
    
    return 0;
}