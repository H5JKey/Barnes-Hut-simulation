#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>

class ParticleSystem {
private:
    std::vector<sf::Vector2f> position;
    std::vector<sf::Vector2f> velocity;
    std::vector<sf::Vector2f> acceleration;
    std::vector<float> mass;
    sf::Vector2u worldSize;
    size_t count;

public:
    ParticleSystem(size_t count, sf::Vector2u worldSize);

    void update(sf::Time& elapsedTime);
    
    void setMass(size_t idx, float mass);
    void accelerate(std::vector<sf::Vector2f> force);
    float getMass(size_t idx) const;

    void setPosition(size_t idx, const sf::Vector2f& position);
    sf::Vector2f getPosition(size_t idx) const;

    void setVelocity(size_t idx, const sf::Vector2f& position);
    sf::Vector2f getVelocity(size_t idx) const;

    size_t getCount() const;

    ~ParticleSystem() = default;
    
private:
    void updatePosition(sf::Time& elapsedTime);
	void updateVelocity(sf::Time& elapsedTime);

    void init();
};