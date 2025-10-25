#pragma once
#include <SFML/Graphics.hpp>

class Particle : sf::Drawable {
private:
    sf::RectangleShape pixel;
    sf::Vector2f position;
    float mass;

    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    void updatePosition(sf::Time& ellapsedTime);
	void updateVelocity(sf::Time& ellapsedTime);

public:
    Particle(const sf::Vector2f& position, float mass);

    void update(sf::Time& ellapsedTime);
    
    void setMass(float mass);
    void accelerate(sf::Vector2f force);
    float getMass() const;

    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;

    void setVelocity(const sf::Vector2f& position);
    sf::Vector2f getVelocity() const;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
    sf::FloatRect getGlobalBounds() const;

    ~Particle() = default;
};