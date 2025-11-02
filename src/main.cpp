#include <SFML/Graphics.hpp>
#include "quad-tree.hpp"
#include "particle.hpp"
#include "physics-engine.hpp"
#include <random>
#include <cmath>
#include <vector>
#include <iostream>

enum {
    PARTICLES_NUM = 3000
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Galaxy Simulation");
    
    QuadTree tree(sf::Vector2u(800, 800));
    PhysicsEngine physics;
    
    std::vector<Particle> particles;
    particles.emplace_back(sf::Vector2f(400, 400), 65000);

    for (size_t i = 0; i < PARTICLES_NUM; i++) {
        float radius = 10 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/300));

        float phi = 2 * 3.14159 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        
        float x = 400 + radius * cos(phi) + (rand() % 20 - 10);
        float y = 400 + radius * sin(phi) + (rand() % 20 - 10);
        
        particles.emplace_back(sf::Vector2f(x, y), rand() % 5 + 1);

        float orbital_velocity = sqrt(10000.0f / radius) * 50;

        sf::Vector2f velocity(orbital_velocity * sin(-phi) + (rand() % 50 - 50), 
                            orbital_velocity * cos(phi) + (rand() % 50 - 50));
        particles.back().setVelocity(velocity);
    }
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        sf::Time elapsedTime = clock.restart();
        if (elapsedTime > sf::seconds(0.005f)) {
            elapsedTime = sf::seconds(0.005f);
        }
        tree.rebuild(particles);
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
        #pragma omp parallel for
        for (int i = 1; i < particles.size(); i++) {
            auto& p = particles[i];
            sf::Vector2f force = tree.calculateForce(p, 0.5f, physics);
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