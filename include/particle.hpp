#pragma once
#include <SFML/Graphics.hpp>

class Particle {
private:
    sf::Vector2f position;
    float mass;

    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    void updatePosition(sf::Time& elapsedTime);
	void updateVelocity(sf::Time& elapsedTime);

public:
    Particle(const sf::Vector2f& position, float mass);

    void update(sf::Time& elapsedTime);
    
    void setMass(float mass);
    void accelerate(sf::Vector2f force);
    float getMass() const;

    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;

    void setVelocity(const sf::Vector2f& position);
    sf::Vector2f getVelocity() const;

    ~Particle() = default;
};