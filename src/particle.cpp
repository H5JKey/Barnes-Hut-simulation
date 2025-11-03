#include "particle.hpp"
#include <iostream>


Particle::Particle(const sf::Vector2f& position, float mass) : velocity(0,0), acceleration(0,0) {
    setPosition(position);
    setMass(mass);
}


void Particle::updatePosition(sf::Time& elapsedTime) {
	this->position += (this->velocity * elapsedTime.asSeconds());
}

void Particle::updateVelocity(sf::Time& elapsedTime) {
	this->velocity += (elapsedTime.asSeconds() * this->acceleration);
}

void Particle::update(sf::Time& elapsedTime) {
    updateVelocity(elapsedTime);
    updatePosition(elapsedTime);
    acceleration = {0,0};
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
}

sf::Vector2f Particle::getPosition() const {
    return position;
}

void Particle::setVelocity(const sf::Vector2f& velocity) {
    this->velocity = velocity;
}

sf::Vector2f Particle::getVelocity() const {
    return velocity;
}