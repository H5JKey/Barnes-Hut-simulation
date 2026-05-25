#include "quad-tree.hpp"
#include <iostream>


size_t QuadTree::size() {
    return nodes.size();
}

QuadTree::Node& QuadTree::getNode(int i) {
    return nodes[i];
}

QuadTree::QuadTree() {
    nodes.resize(0);
    nodes.reserve(100);
}

void QuadTree::rebuild(const ParticleSystem& particles, sf::Vector2u size) {
    nodes.resize(0);
    nodes.reserve(particles.getCount() * 2);
    std::vector<int> idx(particles.getCount());
    for (int i = 0; i < idx.size(); i++) {
        idx[i] = i;
    }
    float root_size = std::min(size.x, size.y);
    buildQuadTree(particles, idx.begin(), idx.end(), sf::Vector2f(root_size / 2, root_size / 2), root_size);
}


int QuadTree::buildQuadTree(const ParticleSystem& particles, std::vector<int>::iterator begin, std::vector<int>::iterator end, sf::Vector2f center, float size) {
    nodes.emplace_back(center, size);
    int currentIdx = nodes.size() - 1;
    if (end - begin == 0) {
        nodes[currentIdx].isLeaf = true;
        nodes[currentIdx].totalMass = 0;
        nodes[currentIdx].centerOfMass = { 0,0 };
        return currentIdx;
    }
    if (end - begin == 1) {
        nodes[currentIdx].particleIndex = *begin;
        nodes[currentIdx].isLeaf = true;
        nodes[currentIdx].totalMass = particles.getMass(*begin);
        nodes[currentIdx].centerOfMass = particles.getPosition(*begin);
        return currentIdx;
    }
    float totalMass = 0;
    sf::Vector2f centerOfMass(0, 0);
    for (auto i = begin; i != end; i++) {
        sf::Vector2f position = particles.getPosition(*i);
        float mass = particles.getMass(*i);
        totalMass += mass;
        centerOfMass += mass * position;
    }
    nodes[currentIdx].totalMass = totalMass;
    nodes[currentIdx].centerOfMass = centerOfMass / totalMass;

    auto mid_y = std::partition(begin, end, [&](int i) {
        return particles.getPosition(i).y < center.y;
        });

    auto nw_end = std::partition(begin, mid_y, [&](int i) {
        return particles.getPosition(i).x < center.x;
        });

    auto sw_end = std::partition(mid_y, end, [&](int i) {
        return particles.getPosition(i).x < center.x;
        });

    float new_size = size / 2;
    if (new_size > 1) {
        nodes[currentIdx].children[0] = buildQuadTree(particles, begin, nw_end, sf::Vector2f(center.x - new_size / 2, center.y - new_size / 2), new_size);
        nodes[currentIdx].children[1] = buildQuadTree(particles, nw_end, mid_y, sf::Vector2f(center.x + new_size / 2, center.y - new_size / 2), new_size);
        nodes[currentIdx].children[2] = buildQuadTree(particles, mid_y, sw_end, sf::Vector2f(center.x - new_size / 2, center.y + new_size / 2), new_size);
        nodes[currentIdx].children[3] = buildQuadTree(particles, sw_end, end, sf::Vector2f(center.x + new_size / 2, center.y + new_size / 2), new_size);
        nodes[currentIdx].isLeaf = false;
    }
    return currentIdx;
}


void QuadTree::draw(sf::RenderWindow& window) const {
    sf::RectangleShape border;
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::White);
    border.setOutlineThickness(1);
    border.setOutlineColor(sf::Color(255, 255, 255, 30));

    for (int i = 0; i < nodes.size(); i++) {
        border.setSize(sf::Vector2f(nodes[i].size, nodes[i].size));
        border.setPosition(nodes[i].center.x - nodes[i].size / 2, nodes[i].center.y - nodes[i].size / 2);
        window.draw(border);
    }
}
