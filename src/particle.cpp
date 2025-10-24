#include "particle.hpp"

Particle::Particle(const sf::Vector2f& position, float mass) : pixel(sf::Vector2f(1,1)), velocity(0,0), acceleration(0,0) {
    setPosition(position);
    setMass(mass);
}

void Particle::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(pixel, states);
}

void Particle::updatePosition(sf::Time& ellapsedTime) {
	this->position += (this->velocity * ellapsedTime.asSeconds());
}

void Particle::updateVelocity(sf::Time& ellapsedTime) {
	this->velocity += (ellapsedTime.asSeconds() * this->acceleration);
}


sf::FloatRect Particle::getGlobalBounds() const {
    return pixel.getGlobalBounds();
}

void Particle::setMass(float mass) {
    this->mass = mass;
}

float Particle::getMass() const {
    return mass;
}

void Particle::setPosition(const sf::Vector2f& position) {
    this->position = position;
    pixel.setPosition(position);
}

sf::Vector2f Particle::getPosition() const {
    return position;
}