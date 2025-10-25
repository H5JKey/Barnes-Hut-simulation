#include "particle.hpp"
#include <iostream>


Particle::Particle(const sf::Vector2f& position, float mass) : pixel(sf::Vector2f(3,3)), velocity(0,0), acceleration(0,0) {
    setPosition(position);
    setMass(mass);
}

void Particle::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(pixel, states);
}

void Particle::updatePosition(sf::Time& ellapsedTime) {
	this->position += (this->velocity * ellapsedTime.asSeconds());
    pixel.setPosition(position);
}

void Particle::updateVelocity(sf::Time& ellapsedTime) {
	this->velocity += (ellapsedTime.asSeconds() * this->acceleration);
}

void Particle::update(sf::Time& ellapsedTime) {
    updateVelocity(ellapsedTime);
    updatePosition(ellapsedTime);
    acceleration = {0,0};
}

sf::FloatRect Particle::getGlobalBounds() const {
    return pixel.getGlobalBounds();
}

void Particle::accelerate(sf::Vector2f force) {
    acceleration += force / mass;
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

void Particle::setVelocity(const sf::Vector2f& velocity) {
    this->velocity = velocity;
    pixel.setPosition(velocity);
}

sf::Vector2f Particle::getVelocity() const {
    return velocity;
}