#include <SFML/Graphics.hpp>
#include "quad-tree.hpp"
#include "particle.hpp"
#include "physics-engine.hpp"
#include <random>
#include <cmath>
#include <vector>
#include <iostream>

enum {
    PARTICLES_NUM = 80000
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 1600), "Galaxy Simulation");
    
    QuadTree tree(window.getSize());
    PhysicsEngine physics;
    
    std::vector<Particle> particles;
    particles.emplace_back(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2), 170000);

    for (size_t i = 0; i < PARTICLES_NUM; i++) {
        float radius = 10 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/300));

        float phi = 2 * 3.14159 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        
        float x = window.getSize().x / 2 + radius * cos(phi) + (rand() % 20 - 10);
        float y = window.getSize().y / 2 + radius * sin(phi) + (rand() % 20 - 10);
        
        particles.emplace_back(sf::Vector2f(x, y), rand() % 1500 + 1);

        float orbital_velocity = sqrt(10000.0f / radius) * 400;

        sf::Vector2f velocity(orbital_velocity * sin(-phi) + (rand() % 50 - 50), 
                            orbital_velocity * cos(phi) + (rand() % 50 - 50));
        particles.back().setVelocity(velocity);
    }
    sf::Clock clock;
    sf::Time av = sf::Time::Zero;
    long long cnt = 0;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        sf::Time elapsedTime = clock.restart();
        av+=elapsedTime;
        cnt++;
        if (elapsedTime > sf::seconds(0.0002f)) {
            elapsedTime = sf::seconds(0.0002f);
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
        tree.rebuild(particles, window.getSize());
        #pragma omp parallel for
        for (int i = 0; i < particles.size(); i++) {
            sf::Vector2f force = tree.calculateForce(i, particles, 0.5f, physics);
            physics.accelerate(particles[i], force);
            particles[i].update(elapsedTime);
        }
        
        window.clear();
        sf::VertexArray particleBodies(sf::Points, particles.size());
        for (int i=0; i < particles.size(); i++) {
            particleBodies[i].position = particles[i].getPosition();
            particleBodies[i].color = sf::Color::White;
        }
        window.draw(particleBodies);
        //tree.draw(window);
        window.display();
    }
    std::cout<<"AVERAGE FRAME TIME: "<<av.asMilliseconds()/cnt<<'\n';
    
    return 0;
}