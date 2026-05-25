#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>


class ParticleSystem {
private:
    std::vector<sf::Color> colors;
    std::vector<float> position_x, position_y;
    std::vector<float> velocity_x, velocity_y;
    std::vector<float> acceleration_x, acceleration_y;
    std::vector<float> mass;
    sf::Vector2u worldSize;
    size_t count;

public:
    ParticleSystem(size_t count, sf::Vector2u worldSize);

    void update(sf::Time& elapsedTime);
    
    void setMass(size_t idx, float mass);
    void accelerate(std::vector<sf::Vector2f> force);
    float getMass(size_t idx) const noexcept;

    sf::Color getColor(size_t idx) const noexcept;
    void setPosition(size_t idx, const sf::Vector2f& position);
    sf::Vector2f getPosition(size_t idx) const noexcept;

    void setVelocity(size_t idx, const sf::Vector2f& position);
    sf::Vector2f getVelocity(size_t idx) const noexcept;

    const std::vector<float>& getPositions_x() const noexcept;
    const std::vector<float>& getPositions_y() const noexcept;
    const std::vector<float>& getMasses() const noexcept;
    size_t getCount() const noexcept;

    ~ParticleSystem() = default;
    
private:

    void init();
};