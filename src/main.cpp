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
    sf::RenderWindow window(sf::VideoMode(800, 800), "Galaxy Simulation");
    
    PhysicsEngine physics;
    sf::View view;
    view.setSize(10000,10000);
    view.setCenter(view.getSize()/2.f);
    BarnesHutCalculator forceCalculator(static_cast<sf::Vector2u>(view.getSize()), &physics, 0.5f);
    
    ParticleSystem particles(PARTICLES_NUM, static_cast<sf::Vector2u>(view.getSize()));
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
            particleBodies[i].color = particles.getColor(i);
        }
        window.setView(view);
        window.draw(particleBodies);
        if (showTree)
            forceCalculator.getQuadTree()->draw(window);
        window.display();
        //window.setView(window.getDefaultView());
    }
    std::cout<<"AVERAGE FRAME TIME: "<<av.asMilliseconds()/cnt<<'\n';
    
    return 0;
}