#include <SFML/Graphics.hpp>
#include "quad-tree.hpp"
#include "particle-system.hpp"
#include "physics-engine.hpp"
#include "force-calculator.hpp"
#include <random>
#include <cmath>
#include <vector>
#include <iostream>

enum {
    PARTICLES_NUM = 100000
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 1600), "Galaxy Simulation");
    
    PhysicsEngine physics;
    BarnesHutCalculator forceCalculator(window.getSize(), &physics, 0.5f);
    
    ParticleSystem particles(PARTICLES_NUM, window.getSize());
    std::vector<sf::Vector2f> forces;

    bool showTree = false;
    
    sf::Clock clock;
    sf::Time av = sf::Time::Zero;
    long long cnt = 0;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::T)
                showTree = !showTree;
            } 
            
        }
        sf::Time elapsedTime = clock.restart();
        av+=elapsedTime;
        cnt++;
        if (elapsedTime > sf::seconds(0.0005f)) {
            elapsedTime = sf::seconds(0.0005f);
        }
        forceCalculator.calculateForces(particles, forces);
        particles.accelerate(forces);
        particles.update(elapsedTime);

        window.clear();
        sf::VertexArray particleBodies(sf::Points, particles.getCount());
        for (int i=0; i < particles.getCount(); i++) {
            particleBodies[i].position = particles.getPosition(i);
            particleBodies[i].color = sf::Color::White;
        }
        window.draw(particleBodies);
        if (showTree)
            forceCalculator.getQuadTree()->draw(window);
        window.display();
    }
    std::cout<<"AVERAGE FRAME TIME: "<<av.asMilliseconds()/cnt<<'\n';
    
    return 0;
}