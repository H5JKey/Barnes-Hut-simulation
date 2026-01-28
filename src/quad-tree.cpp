#include "quad-tree.hpp"
#include <iostream>


size_t QuadTree::size() {
    return nodes.size();
}

QuadTree::Node& QuadTree::getNode(int i){
    return nodes[i];
}

QuadTree::QuadTree() {
    nodes.resize(0);
    nodes.reserve(100);
}

void QuadTree::rebuild(const ParticleSystem& particles, sf::Vector2u size) {
    nodes.resize(0);
    nodes.reserve(particles.getCount()*2);
    std::vector<int> idx(particles.getCount());
    for (int i=0; i<idx.size(); i++) {
        idx[i] = i;
    }
    float root_size = std::min(size.x, size.y);
    buildQuadTree(particles, std::move(idx), sf::Vector2f(root_size / 2, root_size / 2), root_size);
}

int QuadTree::buildQuadTree(const ParticleSystem& particles, const std::vector<int>& idx, sf::Vector2f center, float size) {
    nodes.emplace_back(center, size);
    int currentIdx = nodes.size() - 1;
    if (idx.size() == 0) {
        nodes[currentIdx].isLeaf = true;
        nodes[currentIdx].totalMass = 0;
        nodes[currentIdx].centerOfMass = {0,0};   
        return currentIdx;
    }
    if (idx.size() == 1) {
        nodes[currentIdx].particleIndex = idx[0];
        nodes[currentIdx].isLeaf = true;
        nodes[currentIdx].totalMass = particles.getMass(idx[0]);
        nodes[currentIdx].centerOfMass = particles.getPosition(idx[0]);    
        return currentIdx;
    }
    std::vector<int> nw, ne, sw, se;
    nw.reserve(idx.size() / 4);
    ne.reserve(idx.size() / 4);
    sw.reserve(idx.size() / 4);
    se.reserve(idx.size() / 4);
    float totalMass = 0;
    sf::Vector2f centerOfMass(0,0);
    for (int i=0; i<idx.size(); i++) {
        sf::Vector2f position = particles.getPosition(idx[i]);
        float mass = particles.getMass(idx[i]);

        int quadrant = ((position.x >= center.x) << 1) | (position.y >= center.y);
        switch (quadrant)
        {
        case 0:
            nw.push_back(idx[i]);
            break;
        case 1:
            sw.push_back(idx[i]);
            break;
        case 2:
            ne.push_back(idx[i]);
            break;
        case 3:
            se.push_back(idx[i]);
            break;
        }
        totalMass+=mass;
        centerOfMass+=mass*position;
    }
    nodes[currentIdx].totalMass = totalMass;
    nodes[currentIdx].centerOfMass = centerOfMass / totalMass;

    float new_size = size / 2;
    if (new_size > 1) {
        nodes[currentIdx].children[0] = buildQuadTree(particles, std::move(nw), sf::Vector2f(center.x-new_size / 2, center.y-new_size / 2), new_size);
        nodes[currentIdx].children[1] = buildQuadTree(particles, std::move(ne), sf::Vector2f(center.x+new_size / 2, center.y-new_size / 2), new_size);
        nodes[currentIdx].children[2] = buildQuadTree(particles, std::move(sw), sf::Vector2f(center.x-new_size / 2, center.y+new_size / 2), new_size);
        nodes[currentIdx].children[3] = buildQuadTree(particles, std::move(se), sf::Vector2f(center.x+new_size / 2, center.y+new_size / 2), new_size);
        nodes[currentIdx].isLeaf = false;
    }
    return currentIdx;
 }
void QuadTree::draw(sf::RenderWindow& window) const {
    sf::RectangleShape border;
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::White);
    border.setOutlineThickness(1);
    border.setOutlineColor(sf::Color(255,255,255,30));

    for (int i=0; i<nodes.size(); i++) {
        border.setSize(sf::Vector2f(nodes[i].size, nodes[i].size));
        border.setPosition(nodes[i].center.x - nodes[i].size / 2, nodes[i].center.y - nodes[i].size / 2);
        window.draw(border);
    }
}
