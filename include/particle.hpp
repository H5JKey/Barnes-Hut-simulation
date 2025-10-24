#pragma once
#include <SFML/Graphics.hpp>

class Particle : sf::Drawable {
private:
    sf::RectangleShape pixel;
    sf::Vector2f position;
    float mass;

    sf::Vector2f velocity;
    sf::Vector2f acceleration;

public:
    Particle(const sf::Vector2f& position, float mass);

    void updatePosition(sf::Time& ellapsedTime);
	void updateVelocity(sf::Time& ellapsedTime);
    
    void setMass(float mass);
    float getMass() const;

    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
    sf::FloatRect getGlobalBounds() const;

    ~Particle() = default;
};