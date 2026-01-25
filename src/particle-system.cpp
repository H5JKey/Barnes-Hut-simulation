#include "particle-system.hpp"


ParticleSystem::ParticleSystem(size_t count, sf::Vector2u worldSize) : worldSize(worldSize), count(count) {
    position.resize(count);
    velocity.resize(count);
    acceleration.resize(count);
    mass.resize(count);
    init();
}

void ParticleSystem::init() {
    position[0].x = worldSize.x / 2;
    position[0].y = worldSize.y / 2;
    mass[0] = 175000;
    for (size_t i = 1; i < count; i++) {
        float radius = 10 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/300));

        float phi = 2 * 3.14159 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        
        float x = worldSize.x / 2 + radius * cos(phi) + (rand() % 20 - 10);
        float y = worldSize.y / 2 + radius * sin(phi) + (rand() % 20 - 10);
        
        position[i].x = x;
        position[i].y = y;
        mass[i] = rand() % 1500 + 1;

        float orbital_velocity = sqrt(10000.0f / radius) * 100;

        velocity[i].x = orbital_velocity * sin(-phi) + (rand() % 50 - 50); 
        velocity[i].y = orbital_velocity * cos(phi) + (rand() % 50 - 50);
    }
}


void ParticleSystem::updatePosition(sf::Time& elapsedTime) {
    float dt = elapsedTime.asSeconds();
    #pragma omp parallel for simd 
    for (int i=0; i<count; i++) {
	    position[i].x += velocity[i].x * dt;
        position[i].y += velocity[i].y * dt;
    }
}

void ParticleSystem::updateVelocity(sf::Time& elapsedTime) {
    float dt = elapsedTime.asSeconds();
    #pragma omp parallel for simd 
	for (int i=0; i<count; i++) {
	    velocity[i].x += acceleration[i].x * dt;
        velocity[i].y += acceleration[i].y * dt;
    }
}

void ParticleSystem::update(sf::Time& elapsedTime) {
    updateVelocity(elapsedTime);
    updatePosition(elapsedTime);
    for (int i = count - 1; i >= 0; --i) {
        const auto& pos = position[i];
        if (pos.x < 0 || pos.x > worldSize.x || 
            pos.y < 0 || pos.y > worldSize.y) {
            
            if (i != count - 1) {
                std::swap(position[i], position.back());
                std::swap(mass[i], mass.back());
                std::swap(velocity[i], velocity.back());
                std::swap(acceleration[i], acceleration.back());
            }
            position.pop_back();
            mass.pop_back();
            velocity.pop_back();
            acceleration.pop_back();
            count--;
        }
    }
    #pragma omp parallel for
    for (int i = 0; i < count; i++) {
        acceleration[i] = sf::Vector2f(0, 0);
    }
}

void ParticleSystem::accelerate(std::vector<sf::Vector2f> force) {
    #pragma omp parallel for
    for (int i = 0; i < count; i++) {
        acceleration[i].x += force[i].x / mass[i];
        acceleration[i].y += force[i].y / mass[i];
    }
}

void ParticleSystem::setMass(size_t idx, float mass) {
    this->mass[idx] = mass;
}

float ParticleSystem::getMass(size_t idx) const {
    return mass[idx];
}

void ParticleSystem::setPosition(size_t idx, const sf::Vector2f& position) {
    this->position[idx] = position;
}

sf::Vector2f ParticleSystem::getPosition(size_t idx) const {
    return position[idx];
}

void ParticleSystem::setVelocity(size_t idx, const sf::Vector2f& velocity) {
    this->velocity[idx] = velocity;
}

sf::Vector2f ParticleSystem::getVelocity(size_t idx) const {
    return velocity[idx];
}

size_t ParticleSystem::getCount() const {
    return count;
}