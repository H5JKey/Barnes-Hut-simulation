#include "particle-system.hpp"


ParticleSystem::ParticleSystem(size_t count, sf::Vector2u worldSize) : worldSize(worldSize), count(count) {
    position_x.resize(count);
    position_y.resize(count);
    velocity_x.resize(count);
    velocity_y.resize(count);
    acceleration_x.resize(count);
    acceleration_y.resize(count);
    mass.resize(count);
    init();
}

void ParticleSystem::init() {
    position_x[0] = worldSize.x / 2;
    position_y[0] = worldSize.y / 2;
    mass[0] = 175000;
    for (size_t i = 1; i < count; i++) {
        float radius = 10 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/300));

        float phi = 2 * 3.14159 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        
        float x = worldSize.x / 2 + radius * cos(phi) + (rand() % 20 - 10);
        float y = worldSize.y / 2 + radius * sin(phi) + (rand() % 20 - 10);
        
        position_x[i] = x;
        position_y[i] = y;
        mass[i] = rand() % 1500 + 1;

        float orbital_velocity = sqrt(10000.0f / radius) * 100;

        velocity_x[i] = orbital_velocity * sin(-phi) + (rand() % 50 - 50); 
        velocity_y[i] = orbital_velocity * cos(phi) + (rand() % 50 - 50);
    }
}


void ParticleSystem::update(sf::Time& elapsedTime) {
    float dt = elapsedTime.asSeconds();
    #pragma omp simd
    for (int i=0; i<count; i++) {
        velocity_x[i] += acceleration_x[i] * dt;
        velocity_y[i] += acceleration_y[i] * dt;
        position_x[i] += velocity_x[i] * dt;
        position_y[i] += velocity_y[i] * dt;
        acceleration_x[i] = 0;
        acceleration_y[i] = 0;
    }
    
    for (int i = count - 1; i >= 0; --i) {
        const auto& pos = getPosition(i);
        if (pos.x < 0 || pos.x > worldSize.x || 
            pos.y < 0 || pos.y > worldSize.y) {
            
            if (i != count - 1) {
                std::swap(position_x[i], position_x.back());
                std::swap(position_y[i], position_y.back());
                std::swap(mass[i], mass.back());
                std::swap(velocity_x[i], velocity_x.back());
                std::swap(velocity_y[i], velocity_y.back());
                std::swap(acceleration_x[i], acceleration_x.back());
                std::swap(acceleration_y[i], acceleration_y.back());
            }
            position_x.pop_back();
            position_y.pop_back();
            mass.pop_back();
            velocity_x.pop_back();
            velocity_y.pop_back();
            acceleration_x.pop_back();
            acceleration_y.pop_back();
            count--;
        }
    }
}

void ParticleSystem::accelerate(std::vector<sf::Vector2f> force) {
    #pragma omp simd
    for (int i = 0; i < count; i++) {
        acceleration_x[i] += force[i].x / mass[i];
        acceleration_y[i] += force[i].y / mass[i];
    }
}

void ParticleSystem::setMass(size_t idx, float mass) {
    this->mass[idx] = mass;
}

float ParticleSystem::getMass(size_t idx) const {
    return mass[idx];
}

void ParticleSystem::setPosition(size_t idx, const sf::Vector2f& position) {
    this->position_x[idx] = position.x;
    this->position_y[idx] = position.y;
}

sf::Vector2f ParticleSystem::getPosition(size_t idx) const {
    return {position_x[idx], position_y[idx]};
}

void ParticleSystem::setVelocity(size_t idx, const sf::Vector2f& velocity) {
    this->velocity_x[idx] = velocity.x;
    this->velocity_y[idx] = velocity.y;
}

sf::Vector2f ParticleSystem::getVelocity(size_t idx) const {
    return {velocity_x[idx], velocity_y[idx]};
}

size_t ParticleSystem::getCount() const {
    return count;
}